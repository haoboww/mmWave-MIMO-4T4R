"""
BGT60ATR24C 2T4R 点云可视化脚本

加载 point_cloud_gen_2t4r.py 生成的 NPZ 点云文件，提供 3D 交互式播放。
左侧为 3D 散点图，右侧为 2D 俯视图。

使用方法:
    python visualize_pointcloud_2t4r.py
    python visualize_pointcloud_2t4r.py --pointcloud path/to/pointcloud.npz --fps 15

操作说明:
    空格键: 播放/暂停
    左/右方向键 或 A/D: 上一帧/下一帧
    上/下方向键: 加速/减速
    1/3/5: 聚合1/3/5帧点云
    T: 切换颜色模式 (速度 / 功率)
    P: 显示/隐藏点云
    R: 重置视角
    L: 循环播放
    Q/Escape: 退出
"""

import numpy as np
import argparse
from pathlib import Path
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation


# ============================================================================
# 数据加载
# ============================================================================

def load_pointcloud(npz_path):
    """
    加载 point_cloud_gen_2t4r.py 生成的 NPZ 文件

    Returns:
        list of dicts (每帧: frame_id, x, y, z, velocity, power, num_points)
    """
    data = np.load(npz_path)
    frames_info = data['frames']
    all_points = data['points']

    print(f"点云文件: {npz_path}")
    print(f"  总帧数: {len(frames_info)}")
    print(f"  总点数: {len(all_points)}")

    frames_data = []
    for fi in frames_info:
        offset = fi['offset']
        n = fi['num_points']
        pts = all_points[offset:offset + n]

        frames_data.append({
            'frame_id': int(fi['frame_id']),
            'timestamp': float(fi['timestamp']),
            'x': pts['x'].astype(np.float64),
            'y': pts['y'].astype(np.float64),
            'z': pts['z'].astype(np.float64),
            'velocity': pts['velocity'].astype(np.float64),
            'power': pts['power'].astype(np.float64),
            'range': pts['range'].astype(np.float64),
            'azimuth': pts['azimuth'].astype(np.float64),
            'num_points': int(n),
        })

    return frames_data


# ============================================================================
# 可视化器
# ============================================================================

class Visualizer2T4R:
    def __init__(self, pc_frames, fps=10):
        self.pc_frames = pc_frames
        self.num_frames = len(pc_frames)

        self.current_frame_idx = 0
        self.aggregation_window = 1
        self.fps = fps
        self.playing = True
        self.loop = True
        self.show_pointcloud = True
        self.color_mode = 'velocity'

        self.default_elev = -75
        self.default_azim = -90
        self.current_elev = self.default_elev
        self.current_azim = self.default_azim

        self._compute_ranges()
        self.setup_figure()

    def _compute_ranges(self):
        all_x, all_y, all_v, all_pw = [], [], [], []
        for f in self.pc_frames:
            if f['num_points'] > 0:
                all_x.append(f['x'])
                all_y.append(f['y'])
                all_v.append(f['velocity'])
                all_pw.append(f['power'])

        if all_x:
            ax = np.concatenate(all_x)
            ay = np.concatenate(all_y)
            av = np.concatenate(all_v)
            ap = np.concatenate(all_pw)
            pad = 0.5
            self.x_lim = (ax.min() - pad, ax.max() + pad)
            self.y_lim = (ay.min() - pad, ay.max() + pad)
            self.z_lim = (-1.0, 1.0)
            self.v_lim = (av.min(), av.max())
            self.pw_lim = (ap.min(), ap.max())
            print(f"点云范围  X: [{ax.min():.2f}, {ax.max():.2f}]  "
                  f"Y: [{ay.min():.2f}, {ay.max():.2f}]")
        else:
            self.x_lim = (-5, 5)
            self.y_lim = (0, 8)
            self.z_lim = (-1, 1)
            self.v_lim = (-2, 2)
            self.pw_lim = (0, 1)

    def setup_figure(self):
        plt.style.use('dark_background')
        self.fig = plt.figure(figsize=(16, 9))
        self.fig.canvas.manager.set_window_title('BGT60ATR24C 2T4R Point Cloud Viewer')
        self.fig.canvas.mpl_connect('key_press_event', self.on_key_press)

        self.ax3d = self.fig.add_axes([0.02, 0.08, 0.55, 0.84], projection='3d')
        self.ax2d = self.fig.add_axes([0.62, 0.08, 0.36, 0.84])

        help_text = ('Space: Play/Pause | Left/Right: Frame | Up/Down: Speed | '
                     '1/3/5: Agg | T: Color | P: Cloud | R: Reset view | L: Loop | Q: Quit')
        self.fig.text(0.5, 0.01, help_text, ha='center', fontsize=9, color='gray')
        self.title_text = self.fig.text(0.5, 0.95, '', ha='center', fontsize=12, color='white')

        self.ax3d.view_init(elev=self.default_elev, azim=self.default_azim)

    def _setup_ax3d(self):
        self.ax3d.set_xlabel('X [m]', fontsize=10, labelpad=8)
        self.ax3d.set_ylabel('Y [m] (Range)', fontsize=10, labelpad=8)
        self.ax3d.set_zlabel('Z [m]', fontsize=10, labelpad=8)
        self.ax3d.set_xlim(self.x_lim)
        self.ax3d.set_ylim(self.y_lim)
        self.ax3d.set_zlim(self.z_lim)

    def _setup_ax2d(self):
        self.ax2d.set_xlabel('X [m]', fontsize=10)
        self.ax2d.set_ylabel('Y [m] (Range)', fontsize=10)
        self.ax2d.set_xlim(self.x_lim)
        self.ax2d.set_ylim(self.y_lim)
        self.ax2d.set_aspect('equal')
        self.ax2d.grid(True, alpha=0.3)

    def get_aggregated(self):
        half = (self.aggregation_window - 1) // 2
        c = self.current_frame_idx
        start = max(0, c - half)
        end = min(self.num_frames - 1, c + half)

        arrays = {'x': [], 'y': [], 'z': [], 'velocity': [], 'power': [], 'range': []}
        for i in range(start, end + 1):
            f = self.pc_frames[i]
            if f['num_points'] > 0:
                for k in arrays:
                    arrays[k].append(f[k])

        if arrays['x']:
            return {k: np.concatenate(v) for k, v in arrays.items()}
        return None

    def animate(self, _):
        if self.playing:
            self.current_frame_idx += 1
            if self.current_frame_idx >= self.num_frames:
                if self.loop:
                    self.current_frame_idx = 0
                else:
                    self.current_frame_idx = self.num_frames - 1
                    self.playing = False
        return self.update_plot()

    def update_plot(self):
        self.current_elev = self.ax3d.elev
        self.current_azim = self.ax3d.azim

        self.ax3d.clear()
        self.ax2d.clear()

        agg = self.get_aggregated()
        n_pts = 0

        if self.show_pointcloud and agg is not None:
            x, y, z = agg['x'], agg['y'], agg['z']
            vel, pw = agg['velocity'], agg['power']
            n_pts = len(x)

            if self.color_mode == 'velocity':
                c_val, cmap, clabel = vel, 'coolwarm', 'Velocity'
                vmin, vmax = self.v_lim
            else:
                c_val, cmap, clabel = pw, 'jet', 'Power'
                vmin, vmax = self.pw_lim

            self.ax3d.scatter(
                x, y, z,
                c=c_val, cmap=cmap, s=30, alpha=0.8,
                vmin=vmin, vmax=vmax,
                label=f'{clabel} ({n_pts} pts)'
            )

            self.ax2d.scatter(
                x, y,
                c=c_val, cmap=cmap, s=25, alpha=0.8,
                vmin=vmin, vmax=vmax,
            )
            self.ax2d.set_title(f'Top View ({n_pts} pts)', fontsize=11)
        else:
            self.ax2d.set_title('Top View (no data)', fontsize=11)

        self._setup_ax3d()
        self._setup_ax2d()
        self.ax3d.view_init(elev=self.current_elev, azim=self.current_azim)

        if n_pts > 0:
            self.ax3d.legend(loc='upper right', fontsize=9)

        frame_id = self.pc_frames[self.current_frame_idx]['frame_id']
        status = "Playing" if self.playing else "Paused"
        title = (f'Frame {frame_id} ({self.current_frame_idx + 1}/{self.num_frames})'
                 f'  |  Agg: {self.aggregation_window}  |  Points: {n_pts}'
                 f'  |  {self.fps} FPS  |  {status}'
                 f'  |  Color: {self.color_mode}')
        self.title_text.set_text(title)

        return []

    def on_key_press(self, event):
        if event.key == ' ':
            self.playing = not self.playing
            if not self.playing:
                self.update_plot()
                self.fig.canvas.draw_idle()

        elif event.key in ('right', 'd', 'D'):
            self.playing = False
            self.current_frame_idx = min(self.current_frame_idx + 1, self.num_frames - 1)
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key in ('left', 'a', 'A'):
            self.playing = False
            self.current_frame_idx = max(self.current_frame_idx - 1, 0)
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key == 'up':
            self.fps = min(self.fps + 5, 60)
            self.restart_animation()

        elif event.key == 'down':
            self.fps = max(self.fps - 5, 1)
            self.restart_animation()

        elif event.key in ('1', '3', '5'):
            self.aggregation_window = int(event.key)
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key in ('t', 'T'):
            self.color_mode = 'power' if self.color_mode == 'velocity' else 'velocity'
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key in ('p', 'P'):
            self.show_pointcloud = not self.show_pointcloud
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key in ('r', 'R'):
            self.current_elev = self.default_elev
            self.current_azim = self.default_azim
            self.ax3d.view_init(elev=self.default_elev, azim=self.default_azim)
            self.fig.canvas.draw_idle()

        elif event.key in ('l', 'L'):
            self.loop = not self.loop

        elif event.key in ('q', 'Q', 'escape'):
            if hasattr(self, 'anim') and self.anim.event_source:
                self.anim.event_source.stop()
            plt.close(self.fig)

    def restart_animation(self):
        if hasattr(self, 'anim') and self.anim.event_source:
            self.anim.event_source.stop()
        self.anim = FuncAnimation(
            self.fig, self.animate,
            interval=1000 // self.fps, blit=False, cache_frame_data=False
        )
        self.fig.canvas.draw_idle()

    def show(self):
        print("\n" + "=" * 70)
        print("BGT60ATR24C 2T4R Point Cloud Viewer")
        print("=" * 70)
        print("Controls:")
        print("  Space        : Play/Pause")
        print("  Left/Right   : Previous/Next frame")
        print("  Up/Down      : Increase/Decrease FPS")
        print("  1/3/5        : Aggregation window")
        print("  T            : Toggle color (velocity / power)")
        print("  P            : Toggle point cloud")
        print("  R            : Reset view angle")
        print("  L            : Toggle loop")
        print("  Q/Escape     : Quit")
        print("  Mouse drag   : Rotate 3D view")
        print("=" * 70 + "\n")

        self.anim = FuncAnimation(
            self.fig, self.animate,
            interval=1000 // self.fps, blit=False, cache_frame_data=False
        )
        plt.show()


# ============================================================================
# 主程序
# ============================================================================

def main():
    parser = argparse.ArgumentParser(
        description='Visualize BGT60ATR24C 2T4R radar point cloud (3D)',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument(
        '--pointcloud', type=str,
        default=r'D:\Codes\4.0.1\Radar_2T4R\output\pointclouds\pointcloud_2t4r.npz',
        help='Path to pointcloud NPZ file'
    )
    parser.add_argument('--fps', type=int, default=10, help='Playback FPS')
    parser.add_argument('--start-frame', type=int, default=0, help='Starting frame')
    parser.add_argument('--aggregation', type=int, choices=[1, 3, 5], default=1,
                        help='Initial aggregation window')
    parser.add_argument('--no-loop', action='store_true', help='Disable loop')

    args = parser.parse_args()

    pc_path = Path(args.pointcloud)
    if not pc_path.exists():
        print(f"Error: 文件不存在: {pc_path}")
        return

    print("=" * 70)
    print("加载数据...")
    print("=" * 70)

    pc_frames = load_pointcloud(str(pc_path))
    if not pc_frames:
        print("Error: 无帧数据")
        return

    vis = Visualizer2T4R(pc_frames, fps=args.fps)

    if args.start_frame > 0:
        vis.current_frame_idx = min(args.start_frame, vis.num_frames - 1)
    vis.aggregation_window = args.aggregation
    vis.loop = not args.no_loop

    vis.show()


if __name__ == '__main__':
    main()
