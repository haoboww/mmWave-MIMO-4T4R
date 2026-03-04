"""
BGT60ATR24C 2T4R 毫米波雷达 - 点云生成脚本

数据输入: .npy 文件, shape = (num_frames, 8, 128, 256)
  - 8   = 2TX x 4RX 虚拟天线
  - 128 = chirps per frame
  - 256 = samples per chirp

处理流程:
  Step 1: Range FFT        - Blackman-Harris 窗 + 512点FFT (2x零填充)
  Step 2: Mean Subtract    - 前帧/当前帧均值减法 (去除静态杂波)
  Step 3: Doppler FFT      - Blackman-Harris 窗 + 256点FFT (2x零填充)
  Step 4: ABS SUM          - 8天线幅度求和
  Step 5: SO-CFAR          - 速度维度CFAR检测 + 局部极大值
  Step 6: Angle FFT        - 64点方位角FFT (8虚拟天线, ULA d=lambda/2)
  Step 7: 点云输出         - 球坐标 -> 笛卡尔坐标, 保存NPZ

使用方法:
  1. 修改 main() 中的 data_path 指向你的 .npy 文件
  2. 运行: python point_cloud_gen_2t4r.py
  3. 输出保存到 output/ 目录
"""

import numpy as np
from scipy import signal
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import os
import time
from datetime import datetime


# ============================================================================
# 雷达参数配置
# ============================================================================

class RadarConfig2T4R:
    """BGT60ATR24C 2T4R 雷达参数"""

    def __init__(self):
        self.c = 3e8

        # RF 参数
        self.bandwidth = 3e9        # Hz
        self.fc = 59.5e9            # Hz (中心频率)
        self.Tc_rep = 500e-6        # s  (chirp 重复间隔)
        self.Tf = 200e-3            # s  (帧时间)
        self.fs = 1e6               # Hz (ADC 采样率)

        # 天线
        self.num_tx = 2
        self.num_rx = 4
        self.num_ant = 8            # 虚拟天线 = 2*4

        # 采样
        self.num_samples = 256      # 每chirp采样点
        self.num_chirps = 128       # 每帧chirp数

        # FFT 点数 (2x 零填充)
        self.N_range = self.num_samples * 2     # 512
        self.N_doppler = self.num_chirps * 2    # 256
        self.N_angle = 64                       # 方位角 FFT 点数
        self.use_range = self.num_samples       # 取正频率一半

        # CFAR 参数 (可根据场景调整)
        self.cfar_guard = 2
        self.cfar_search = 8
        self.cfar_threshold = 2
        self.cfar_max_num = 400

        self.skip_bins = 3          # 跳过近距离 bin (直流泄漏)

        # 均值减法模式: 0=不减, 1=减当前帧, 2=减前帧
        self.mean_en = 1

        # 物理参数 (自动计算)
        self.Tc = self.num_samples / self.fs
        self.S = self.bandwidth / self.Tc       # 调频斜率
        self.lambda_ = self.c / self.fc
        self.range_res = self.c / (2 * self.bandwidth)
        self.R_max = (self.fs * self.c) / (2 * self.S) / 2
        self.R_bin = self.R_max / self.use_range
        self.v_max = self.lambda_ / (4 * self.Tc_rep)
        self.v_res = 2 * self.v_max / self.N_doppler
        self.angle_res_deg = np.degrees(np.arcsin(2 / self.N_angle))

    def print_params(self):
        print("=" * 70)
        print("BGT60ATR24C 2T4R 雷达参数")
        print("=" * 70)
        print(f"带宽: {self.bandwidth/1e9:.1f} GHz | 中心频率: {self.fc/1e9:.1f} GHz")
        print(f"采样率: {self.fs/1e6:.1f} MHz | 采样点: {self.num_samples}")
        print(f"Chirps/帧: {self.num_chirps} | Chirp间隔: {self.Tc_rep*1e6:.0f} us")
        print(f"帧时间: {self.Tf*1e3:.0f} ms")
        print(f"天线: {self.num_tx}T{self.num_rx}R = {self.num_ant} 虚拟天线")
        print(f"FFT: Range={self.N_range}pt, Doppler={self.N_doppler}pt, Angle={self.N_angle}pt")
        print(f"距离分辨率: {self.range_res*100:.1f} cm | 最大距离: {self.R_max:.1f} m")
        print(f"速度分辨率: {self.v_res:.4f} m/s | 最大速度: +/-{self.v_max:.2f} m/s")
        print(f"角度分辨率: ~{self.angle_res_deg:.1f} deg")
        print(f"CFAR: SO, Guard={self.cfar_guard}, Search={self.cfar_search}, "
              f"Threshold={self.cfar_threshold}")
        print("=" * 70)


# ============================================================================
# Step 1: Range FFT
# ============================================================================

def compute_range_fft(data, range_window, cfg):
    """
    DC去除 -> 加窗 -> 零填充 -> FFT -> 取正频率

    Args:
        data: [num_chirps, num_samples] 单天线原始数据
        range_window: [1, num_samples]
        cfg: RadarConfig2T4R
    Returns:
        [num_chirps, use_range] 复数 Range FFT
    """
    nc, ns = data.shape
    avgs = np.mean(data, axis=1, keepdims=True)
    data = data - avgs
    data = data * range_window
    data_zp = np.pad(data, ((0, 0), (0, ns)), 'constant')
    fft1d = np.fft.fft(data_zp, axis=1) / ns
    fft1d = 2 * fft1d[:, :ns]
    return fft1d


# ============================================================================
# Step 3: Doppler FFT
# ============================================================================

def compute_doppler_fft(fft1d, mean_data, doppler_window, cfg):
    """
    均值减法 -> 转置 -> 加窗 -> 零填充 -> FFT -> FFTshift

    Args:
        fft1d: [num_chirps, use_range]
        mean_data: [use_range] 或 None
        doppler_window: [1, num_chirps]
        cfg: RadarConfig2T4R
    Returns:
        [use_range, N_doppler] 复数 RD Map (零速在中心)
    """
    if mean_data is not None:
        fft1d = fft1d - mean_data.reshape(1, -1)

    data_t = fft1d.T
    data_t = data_t * doppler_window
    data_zp = np.pad(data_t, ((0, 0), (0, cfg.num_chirps)), 'constant')
    fft2d = np.fft.fft(data_zp, axis=1) / cfg.num_chirps
    fft2d = np.fft.fftshift(fft2d, axes=1)
    return fft2d


# ============================================================================
# Step 5: SO-CFAR
# ============================================================================

def cfar_1d_so(signal_in, guard, search, threshold_mul):
    """向量化 1D SO-CFAR (循环边界)"""
    n = len(signal_in)
    pad = guard + search
    ext = np.concatenate([signal_in[-pad:], signal_in, signal_in[:pad]])
    cs = np.cumsum(np.insert(ext, 0, 0.0))

    idx = np.arange(n) + pad
    left_sums = cs[idx - guard] - cs[idx - guard - search]
    right_sums = cs[idx + guard + search + 1] - cs[idx + guard + 1]

    noise = np.minimum(left_sums, right_sums) / search
    return noise * threshold_mul


def cfar_detect_2d(rd_abs_sum, cfg):
    """
    2D CFAR: Doppler 维度 SO-CFAR + Range/Doppler 局部极大值

    Returns:
        list of dict: range_idx, vel_idx, power
    """
    nr, nv = rd_abs_sum.shape
    detections = []

    for r_idx in range(cfg.skip_bins, nr):
        row = rd_abs_sum[r_idx]
        thresh = cfar_1d_so(row, cfg.cfar_guard, cfg.cfar_search,
                            cfg.cfar_threshold)

        for v_idx in range(nv):
            if row[v_idx] <= thresh[v_idx]:
                continue
            is_max = True
            for dv in (-1, 1):
                if row[v_idx] < row[(v_idx + dv) % nv]:
                    is_max = False
                    break
            if is_max:
                for dr in (-1, 1):
                    nr2 = r_idx + dr
                    if 0 <= nr2 < nr and rd_abs_sum[nr2, v_idx] > row[v_idx]:
                        is_max = False
                        break
            if not is_max:
                continue
            detections.append({
                'range_idx': r_idx,
                'vel_idx': v_idx,
                'power': row[v_idx],
            })

    if len(detections) > cfg.cfar_max_num:
        detections.sort(key=lambda d: d['power'], reverse=True)
        detections = detections[:cfg.cfar_max_num]
        detections.sort(key=lambda d: d['range_idx'])

    return detections


# ============================================================================
# Step 6: Angle FFT (方位角)
# ============================================================================

def estimate_azimuth(rd_maps, range_idx, vel_idx, cfg):
    """
    8 虚拟天线 -> N_angle 点 FFT -> 峰值

    Returns:
        list of (az_idx, power)
    """
    az_data = rd_maps[:, range_idx, vel_idx]
    az_fft = np.fft.fft(az_data, n=cfg.N_angle)
    mag = np.abs(az_fft)
    peak_idx = np.argmax(mag)
    return [(peak_idx, float(mag[peak_idx]))]


def idx_to_angle(idx, N):
    """FFT bin 索引 -> 角度 (度), 假设 d = lambda/2"""
    sin_theta = idx * 2 / N if idx < N / 2 else (idx - N) * 2 / N
    sin_theta = np.clip(sin_theta, -1.0, 1.0)
    return np.degrees(np.arcsin(sin_theta))


# ============================================================================
# 帧处理器
# ============================================================================

class FrameProcessor:
    def __init__(self, cfg: RadarConfig2T4R):
        self.cfg = cfg
        self.range_window = signal.windows.blackmanharris(
            cfg.num_samples).reshape(1, cfg.num_samples)
        self.doppler_window = signal.windows.blackmanharris(
            cfg.num_chirps).reshape(1, cfg.num_chirps)
        self.mean_prev = None
        self.mean_cur = None

    def reset(self):
        self.mean_prev = None
        self.mean_cur = None

    def process_frame(self, frame_data):
        """
        处理单帧 [num_ant, num_chirps, num_samples]

        Returns:
            points (list[dict]), stats (dict), rd_abs_sum (ndarray)
        """
        cfg = self.cfg
        na = frame_data.shape[0]

        # --- Step 1: Range FFT ---
        fft1d_all = np.zeros((na, cfg.num_chirps, cfg.use_range),
                             dtype=np.complex128)
        for ant in range(na):
            fft1d_all[ant] = compute_range_fft(
                frame_data[ant].astype(np.float64),
                self.range_window, cfg)

        # --- Step 2: Mean ---
        mean_new = np.mean(fft1d_all, axis=1)
        if cfg.mean_en == 0:
            mean_sub = None
        elif cfg.mean_en == 1:
            mean_sub = mean_new
        else:
            mean_sub = self.mean_prev if self.mean_prev is not None else mean_new
        self.mean_prev = self.mean_cur
        self.mean_cur = mean_new

        # --- Step 3: Doppler FFT ---
        rd_maps = np.zeros((na, cfg.use_range, cfg.N_doppler),
                           dtype=np.complex128)
        for ant in range(na):
            ms = mean_sub[ant] if mean_sub is not None else None
            rd_maps[ant] = compute_doppler_fft(
                fft1d_all[ant], ms, self.doppler_window, cfg)

        # --- Step 4: ABS SUM ---
        rd_abs_sum = np.sum(np.abs(rd_maps), axis=0)

        # --- Step 5: CFAR ---
        detections = cfar_detect_2d(rd_abs_sum, cfg)

        # --- Step 6+7: Angle -> Point Cloud ---
        points = []
        for det in detections:
            r_idx = det['range_idx']
            v_idx = det['vel_idx']
            az_peaks = estimate_azimuth(rd_maps, r_idx, v_idx, cfg)

            for az_idx, az_pow in az_peaks:
                range_val = r_idx * cfg.R_bin
                vel_val = (v_idx - cfg.N_doppler / 2) * cfg.v_res
                az_deg = idx_to_angle(az_idx, cfg.N_angle)

                az_rad = np.radians(az_deg)
                x = range_val * np.sin(az_rad)
                y = range_val * np.cos(az_rad)

                points.append({
                    'x': x, 'y': y, 'z': 0.0,
                    'range': range_val, 'velocity': vel_val,
                    'azimuth': az_deg, 'elevation': 0.0,
                    'power': az_pow,
                    'range_idx': r_idx, 'vel_idx': v_idx,
                    'az_idx': az_idx,
                })

        stats = {
            'cfar_detections': len(detections),
            'final_points': len(points),
        }
        return points, stats, rd_abs_sum


# ============================================================================
# 保存 & 可视化
# ============================================================================

def save_npz(all_frames, all_points, timestamps, filepath):
    """保存为 NPZ (与 point_cloud_gen.py 格式兼容)"""
    if not all_points:
        print("无点云数据, 跳过保存")
        return
    pt_dtype = np.dtype([
        ('x', '<f4'), ('y', '<f4'), ('z', '<f4'),
        ('range', '<f4'), ('velocity', '<f4'),
        ('azimuth', '<f4'), ('elevation', '<f4'), ('power', '<f4'),
    ])
    fr_dtype = np.dtype([
        ('frame_id', '<i4'), ('timestamp', '<f8'),
        ('num_points', '<i4'), ('offset', '<i4'),
    ])

    pts = np.zeros(len(all_points), dtype=pt_dtype)
    for i, p in enumerate(all_points):
        pts[i] = (p['x'], p['y'], p['z'], p['range'],
                  p['velocity'], p['azimuth'], p['elevation'], p['power'])

    frs = np.zeros(len(all_frames), dtype=fr_dtype)
    for i, f in enumerate(all_frames):
        ts_val = (timestamps[f['frame_id']]
                  if timestamps is not None and f['frame_id'] < len(timestamps)
                  else f['frame_id'] * 0.2)
        frs[i] = (f['frame_id'], ts_val, f['num_points'], f['offset'])

    np.savez(filepath, points=pts, frames=frs)
    print(f"点云已保存: {filepath}  ({len(pts)} 点, {len(frs)} 帧)")


def save_rdmap_image(rd_abs_sum, frame_id, output_dir, cfg):
    """保存 Range-Doppler Map 图像"""
    fig, ax = plt.subplots(figsize=(10, 5))
    extent = [-cfg.v_max, cfg.v_max,
              cfg.skip_bins * cfg.R_bin, cfg.R_max]
    data_db = 20 * np.log10(rd_abs_sum[cfg.skip_bins:, :] + 1e-10)
    im = ax.imshow(data_db, aspect='auto', origin='lower',
                   cmap='jet', extent=extent)
    ax.set_xlabel('Velocity (m/s)')
    ax.set_ylabel('Range (m)')
    ax.set_title(f'Range-Doppler Map - Frame {frame_id}')
    plt.colorbar(im, ax=ax, label='Power (dB)')
    filepath = os.path.join(output_dir, f"rdmap_{frame_id:04d}.png")
    plt.savefig(filepath, dpi=100, bbox_inches='tight')
    plt.close()


def save_frame_image(points, frame_id, cfg, filepath):
    """保存单帧点云图"""
    if not points:
        return
    x = np.array([p['x'] for p in points])
    y = np.array([p['y'] for p in points])
    vel = np.array([p['velocity'] for p in points])
    rng = np.array([p['range'] for p in points])
    pw = np.array([p['power'] for p in points])

    fig, axes = plt.subplots(1, 3, figsize=(18, 5))

    sc0 = axes[0].scatter(x, y, c=pw, cmap='jet', s=15)
    axes[0].set_xlabel('X (m)')
    axes[0].set_ylabel('Y (m)')
    axes[0].set_title(f'Top View - Frame {frame_id} ({len(points)} pts)')
    axes[0].grid(True)
    axes[0].set_aspect('equal')
    plt.colorbar(sc0, ax=axes[0], label='Power')

    sc1 = axes[1].scatter(rng, vel, c=pw, cmap='jet', s=15)
    axes[1].set_xlabel('Range (m)')
    axes[1].set_ylabel('Velocity (m/s)')
    axes[1].set_title('Range-Velocity')
    axes[1].grid(True)
    plt.colorbar(sc1, ax=axes[1], label='Power')

    axes[2].plot(rng, pw, '.', markersize=4)
    axes[2].set_xlabel('Range (m)')
    axes[2].set_ylabel('Power')
    axes[2].set_title('Range Profile')
    axes[2].grid(True)

    plt.tight_layout()
    plt.savefig(filepath, dpi=150, bbox_inches='tight')
    plt.close()


# ============================================================================
# 主程序
# ============================================================================

def main():
    # ======== 配置 ========
    cfg = RadarConfig2T4R()
    cfg.print_params()

    # ======== 路径 (按需修改) ========
    data_path = r"D:\Codes\4.0.1\Radar_2T4R\BGT60ATR24C_session2.npy"
    output_base = r"D:\Codes\4.0.1\Radar_2T4R\output"

    start_frame = 0
    end_frame = None        # None = 全部
    save_rdmap_flag = True
    rdmap_save_limit = 5

    rdmap_dir = os.path.join(output_base, "rdmaps")
    pc_dir = os.path.join(output_base, "pointclouds")
    stats_dir = os.path.join(output_base, "stats")
    for d in (rdmap_dir, pc_dir, stats_dir):
        os.makedirs(d, exist_ok=True)

    # ======== 加载数据 ========
    print(f"\n加载: {data_path}")
    obj = np.load(data_path, allow_pickle=True).item()
    data = obj['data']
    timestamps = obj.get('timestamps', None)

    print(f"数据形状: {data.shape}  dtype: {data.dtype}")
    if timestamps is not None:
        print(f"时间戳: {timestamps.shape}")

    num_frames = data.shape[0]
    if end_frame is None:
        end_frame = num_frames
    end_frame = min(end_frame, num_frames)
    print(f"处理帧: {start_frame} ~ {end_frame - 1}  (共 {end_frame - start_frame} 帧)")

    # ======== 处理 ========
    print("\n" + "=" * 70)
    print("开始点云生成")
    print("=" * 70)

    processor = FrameProcessor(cfg)
    all_points = []
    all_frames = []
    all_stats = []
    offset = 0

    t0 = time.time()
    for fi in range(start_frame, end_frame):
        frame_data = data[fi]
        points, stats, rd_abs = processor.process_frame(frame_data)

        all_frames.append({
            'frame_id': fi,
            'num_points': len(points),
            'offset': offset,
        })
        all_points.extend(points)
        offset += len(points)
        all_stats.append(stats)

        if save_rdmap_flag and (fi - start_frame) < rdmap_save_limit:
            save_rdmap_image(rd_abs, fi, rdmap_dir, cfg)

        if fi % 10 == 0 or fi == end_frame - 1:
            elapsed_so_far = time.time() - t0
            print(f"  帧 {fi:3d}: CFAR={stats['cfar_detections']:3d}, "
                  f"点数={stats['final_points']:3d}  "
                  f"({elapsed_so_far:.1f}s)")

    elapsed = time.time() - t0
    total_pts = len(all_points)
    print(f"\n处理完成: {end_frame - start_frame} 帧, "
          f"{total_pts} 点, 耗时 {elapsed:.1f}s "
          f"({(end_frame - start_frame) / max(elapsed, 0.01):.1f} 帧/s)")

    # ======== 保存 ========
    npz_file = os.path.join(pc_dir, "pointcloud_2t4r.npz")
    save_npz(all_frames, all_points, timestamps, npz_file)

    stats_file = os.path.join(stats_dir, "statistics.txt")
    with open(stats_file, 'w', encoding='utf-8') as f:
        f.write(f"生成时间: {datetime.now()}\n")
        f.write(f"数据文件: {data_path}\n")
        f.write(f"总帧数: {len(all_stats)}, 总点数: {total_pts}\n")
        f.write(f"处理耗时: {elapsed:.1f}s\n\n")
        f.write(f"{'帧':>4}  {'CFAR':>6}  {'最终':>6}\n")
        f.write("-" * 22 + "\n")
        for i, s in enumerate(all_stats):
            f.write(f"{start_frame + i:4d}  {s['cfar_detections']:6d}  "
                    f"{s['final_points']:6d}\n")
    print(f"统计已保存: {stats_file}")

    # ======== 可视化 (保存前几帧) ========
    if all_points:
        vis_frames = [0]
        if len(all_frames) > 10:
            vis_frames.append(10)
        if len(all_frames) > 1:
            vis_frames.append(len(all_frames) - 1)
        vis_frames = sorted(set(vis_frames))

        for fi_vis in vis_frames:
            f_info = all_frames[fi_vis]
            pts = all_points[f_info['offset']:
                             f_info['offset'] + f_info['num_points']]
            if pts:
                img_path = os.path.join(
                    pc_dir, f"frame_{f_info['frame_id']:04d}.png")
                save_frame_image(pts, f_info['frame_id'], cfg, img_path)
                print(f"图像: {img_path}")

    print(f"\n输出目录: {output_base}")
    print(f"  pointclouds/ - 点云 NPZ + 帧图像")
    print(f"  rdmaps/      - Range-Doppler Map 图像")
    print(f"  stats/       - 检测统计")
    return all_frames, all_points


if __name__ == "__main__":
    frames, points = main()
