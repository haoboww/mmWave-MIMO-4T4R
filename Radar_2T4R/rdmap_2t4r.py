"""
BGT60ATR24C 2T4R Range-Doppler Map 生成与可视化

用于快速查看单帧/多帧的 Range FFT 和 Range-Doppler Map。
支持 MTI (Moving Target Indicator) 滤波。

数据输入: .npy 文件, shape = (num_frames, 8, 128, 256)

使用方法:
    python rdmap_2t4r.py
"""

import numpy as np
from scipy import signal
from scipy.signal import find_peaks
import matplotlib.pyplot as plt


# ============================================================================
# Range-Doppler 计算引擎
# ============================================================================

class DopplerAlgo:
    """Range-Doppler Map 计算器 (单天线)"""

    def __init__(self, config: dict, num_ant: int, mti_alpha: float = 0.8):
        """
        Args:
            config: 含 num_chirps_per_frame, num_samples_per_chirp
            num_ant: 天线数 (用于 MTI 历史缓存)
            mti_alpha: MTI 滤波器系数 (0=不滤波, 1=完全减去历史)
        """
        self.num_chirps_per_frame = config["num_chirps_per_frame"]
        num_samples_per_chirp = config["num_samples_per_chirp"]

        self.range_window = signal.windows.blackmanharris(
            num_samples_per_chirp).reshape(1, num_samples_per_chirp)
        self.doppler_window = signal.windows.blackmanharris(
            self.num_chirps_per_frame).reshape(1, self.num_chirps_per_frame)

        self.mti_alpha = mti_alpha
        self.mti_history = np.zeros(
            (self.num_chirps_per_frame, num_samples_per_chirp, num_ant))

    def compute_doppler_map(self, data: np.ndarray, i_ant: int):
        """
        计算单天线的 Range-Doppler Map

        Args:
            data: [num_chirps, num_samples] 原始 ADC 数据
            i_ant: 天线索引 (用于 MTI 缓存)

        Returns:
            range_fft: [num_chirps, num_samples] 复数 Range FFT
            rd_map: [num_samples, 2*num_chirps] 复数 RD Map (零速居中)
        """
        data = data - np.average(data)

        # MTI: 减去指数加权历史
        data_mti = data - self.mti_history[:, :, i_ant]
        self.mti_history[:, :, i_ant] = (
            data * self.mti_alpha +
            self.mti_history[:, :, i_ant] * (1 - self.mti_alpha)
        )

        # Range FFT
        range_fft = self._fft_spectrum(data_mti, self.range_window)

        # Doppler FFT
        fft1d_t = np.transpose(range_fft)
        fft1d_t = np.multiply(fft1d_t, self.doppler_window)
        zp2 = np.pad(fft1d_t, ((0, 0), (0, self.num_chirps_per_frame)), "constant")
        fft2d = np.fft.fft(zp2) / self.num_chirps_per_frame

        return range_fft, np.fft.fftshift(fft2d, (1,))

    def _fft_spectrum(self, mat, range_window):
        """DC去除 -> 加窗 -> 零填充 -> FFT -> 取正频率半边"""
        numchirps, chirpsamples = mat.shape
        avgs = np.average(mat, 1).reshape(numchirps, 1)
        mat = mat - avgs
        mat = np.multiply(mat, range_window)
        zp1 = np.pad(mat, ((0, 0), (0, chirpsamples)), 'constant')
        range_fft = np.fft.fft(zp1) / chirpsamples
        range_fft = 2 * range_fft[:, :chirpsamples]
        return range_fft


# ============================================================================
# 雷达参数
# ============================================================================

# BGT60ATR24C 2T4R 配置
NUM_CHIRPS = 128
NUM_SAMPLES = 256
B = 3e9            # Hz (带宽)
FC = 59.5e9        # Hz (中心频率)
TC_REP = 500e-6    # s  (chirp 重复间隔)
TF = 200e-3        # s  (帧时间)
FS = 1e6           # Hz (ADC 采样率)
C = 3e8            # m/s

# 派生参数
N_RANGE = NUM_SAMPLES * 2
N_DOPPLER = NUM_CHIRPS * 2
TC = NUM_SAMPLES / FS
S = B / TC
DELTA_R = C / (2 * B)
R_MAX = (FS * C) / (2 * S) / 2
R_BIN = DELTA_R / (N_RANGE / NUM_SAMPLES)
LAMBDA_ = C / FC
V_MAX = LAMBDA_ / (4 * TC_REP)
DELTA_V = LAMBDA_ / (2 * TF)


# ============================================================================
# 主程序
# ============================================================================

def main():
    # ======== 数据加载 ========
    data_path = r"D:\Codes\4.0.1\Radar_2T4R\BGT60ATR24C_session2.npy"

    obj = np.load(data_path, allow_pickle=True).item()
    data = obj['data']
    ts = obj['timestamps']

    print(f"data: {data.shape} {data.dtype}")
    print(f"timestamps: {ts.shape} {ts.dtype}")

    raw_data = data

    # ======== 初始化算法 ========
    the_ant = 0         # 选择天线
    num_antennas = 3    # MTI 缓存天线数

    config = {
        "num_chirps_per_frame": NUM_CHIRPS,
        "num_samples_per_chirp": NUM_SAMPLES,
    }
    algo = DopplerAlgo(config, num_antennas)

    # ======== 轴坐标 ========
    freq_axis = np.linspace(0, FS / 2, N_RANGE // 2)
    range_axis = (freq_axis * C) / (2 * S)
    doppler_axis = np.linspace(-V_MAX, V_MAX, N_DOPPLER)

    print(f"距离分辨率: {DELTA_R*100:.1f} cm | 最大距离: {R_MAX:.1f} m")
    print(f"速度分辨率: {DELTA_V:.4f} m/s | 最大速度: +/-{V_MAX:.2f} m/s")

    # ======== 处理帧 ========
    start_frame = 0
    end_frame = 1
    skip_bins = 15
    R_start = (skip_bins / (N_RANGE / 2)) * R_MAX

    true_distance = 1.95
    estimated_distances = []

    for frame_idx in range(start_frame, end_frame):
        chirp_idx = 0
        i_ant = 2

        data_one_ant = raw_data[frame_idx, the_ant, :, :]
        fft_1d, rd_map = algo.compute_doppler_map(data_one_ant, i_ant)

        range_fft_onechirp = fft_1d[chirp_idx, :]
        rd_map = np.transpose(rd_map)

        # --- Range FFT (全部) ---
        plt.figure(figsize=(8, 4))
        plt.plot(range_axis, np.abs(range_fft_onechirp), linewidth=2)
        plt.xlabel("Range (m)")
        plt.ylabel("Amplitude")
        plt.title(f"Range FFT - Frame {frame_idx}")
        plt.grid(True)
        plt.xlim(0, R_MAX)
        plt.show()

        # --- Range FFT (跳过近距离) ---
        range_axis_skip = range_axis[skip_bins:]
        range_fft_skip = range_fft_onechirp[skip_bins:]

        plt.figure(figsize=(8, 4))
        plt.plot(range_axis_skip, np.abs(range_fft_skip), linewidth=2)
        plt.xlabel("Range (m)")
        plt.ylabel("Amplitude")
        plt.title(f"Range FFT (skip {skip_bins} bins) - Frame {frame_idx}")
        plt.grid(True)
        plt.xlim(R_start, R_MAX)
        plt.show()

        # --- Range-Doppler Map ---
        rd_map_skip = rd_map[:, skip_bins:]
        plt.figure()
        plt.imshow(
            np.abs(rd_map_skip),
            extent=[R_start, R_MAX, -V_MAX, V_MAX],
            aspect='auto', origin='lower'
        )
        plt.title(f"Range-Doppler Map - Frame {frame_idx}, Ant {i_ant}")
        plt.xlabel("Range (m)")
        plt.ylabel("Velocity (m/s)")
        plt.colorbar(label="Magnitude")
        plt.show()

        # --- 峰值检测 ---
        range_fft_skip_abs = np.abs(range_fft_skip)
        threshold = np.mean(range_fft_skip_abs) + 3 * np.std(range_fft_skip_abs)
        peaks, _ = find_peaks(range_fft_skip_abs, height=threshold)

        if len(peaks) > 0:
            k_max = peaks[np.argmax(range_fft_skip_abs[peaks])]
            R_est = (k_max + skip_bins) * R_BIN
            estimated_distances.append(R_est)
        else:
            estimated_distances.append(np.nan)

    # ======== 结果统计 ========
    estimated_distances = np.array(estimated_distances)
    N = end_frame - start_frame

    plt.figure()
    plt.scatter(np.arange(1, N + 1), estimated_distances, s=10, label='Estimated')
    plt.axhline(true_distance, color='r', linestyle='--', label='True Distance')
    plt.xlabel('Frame Index')
    plt.ylabel('Distance (m)')
    plt.title('Range Estimation')
    plt.legend()
    plt.grid(True)
    plt.show()

    valid = estimated_distances[~np.isnan(estimated_distances)]
    if len(valid) > 0:
        errors = np.abs(valid - true_distance)
        print(f"Mean Error: {np.mean(errors):.4f} m")
        print(f"Std Error:  {np.std(errors):.4f} m")
        if len(errors) >= 10:
            print(f"90% Error:  {np.percentile(errors, 90):.4f} m")


if __name__ == "__main__":
    main()
