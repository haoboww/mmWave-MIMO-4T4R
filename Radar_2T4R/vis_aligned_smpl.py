"""
对齐点云 + SMPL骨架 可视化脚本

将 point_cloud_gen_aligned.py 生成的原始雷达点云转换到相机坐标系，
并与 smpl_target/ 中的SMPL骨架一起可视化。

坐标转换说明:
    point_cloud_gen_aligned.py 输出 (雷达原生坐标):
        pc_x = range * cos(el) * sin(az)   → 横向 (radar_Y = right)
        pc_y = range * cos(el) * cos(az)   → 前方 (radar_X = forward)
        pc_z = range * sin(el)             → 上方 (radar_Z = up)

    转换到相机坐标系 (与 export_pose_pointcloud.py 一致):
        cam_x = -pc_x                     → 横向 (镜像修正)
        cam_y = -pc_z                     → 向下 (camera Y = down)
        cam_z = pc_y                      → 深度 (camera Z = forward)
        + offset                          → 雷达-相机物理偏移

操作说明:
    空格键: 播放/暂停
    左/右方向键 或 A/D: 上一帧/下一帧
    上/下方向键: 加速/减速
    1/3/5: 聚合1/3/5帧点云
    [/]: 调整点云帧偏移 (同步校正)
    0: 重置帧偏移
    P: 显示/隐藏点云
    S: 显示/隐藏骨架
    R: 重置视角
    L: 循环播放
    Q/Escape: 退出
"""

import numpy as np
import json
import os
import glob
import argparse
from pathlib import Path
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation


# ============================================================================
# SMPL 骨架定义
# ============================================================================

# SMPL 24关节的父关节索引
SMPL_PARENTS = [-1, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 12, 13, 14, 16, 17, 18, 19, 20, 21]

# 关节名称
SMPL_JOINT_NAMES = [
    'pelvis', 'left_hip', 'right_hip', 'spine1', 'left_knee', 'right_knee',
    'spine2', 'left_ankle', 'right_ankle', 'spine3', 'left_foot', 'right_foot',
    'neck', 'left_collar', 'right_collar', 'head', 'left_shoulder', 'right_shoulder',
    'left_elbow', 'right_elbow', 'left_wrist', 'right_wrist', 'left_hand', 'right_hand'
]

# 骨骼连接 (绘制用)
SMPL_BONES = [
    (0, 1), (0, 2), (0, 3),       # pelvis -> hips, spine
    (1, 4), (2, 5),               # hips -> knees
    (4, 7), (5, 8),               # knees -> ankles
    (7, 10), (8, 11),             # ankles -> feet
    (3, 6), (6, 9),               # spine chain
    (9, 12), (12, 15),            # spine -> neck -> head
    (9, 13), (9, 14),             # spine -> collars
    (13, 16), (14, 17),           # collars -> shoulders
    (16, 18), (17, 19),           # shoulders -> elbows
    (18, 20), (19, 21),           # elbows -> wrists
    (20, 22), (21, 23),           # wrists -> hands
]

# SMPL 骨骼的静态偏移 (父关节到子关节的相对位置, 米)
SMPL_JOINT_OFFSETS = np.array([
    [0, 0, 0],              # 0 pelvis (root)
    [0.08, -0.06, 0],       # 1 left_hip
    [-0.08, -0.06, 0],      # 2 right_hip
    [0, 0.12, 0],           # 3 spine1
    [0, -0.40, 0],          # 4 left_knee
    [0, -0.40, 0],          # 5 right_knee
    [0, 0.12, 0],           # 6 spine2
    [0, -0.42, 0],          # 7 left_ankle
    [0, -0.42, 0],          # 8 right_ankle
    [0, 0.12, 0],           # 9 spine3
    [0, -0.08, 0.08],       # 10 left_foot
    [0, -0.08, 0.08],       # 11 right_foot
    [0, 0.12, 0],           # 12 neck
    [0.04, 0, 0],           # 13 left_collar
    [-0.04, 0, 0],          # 14 right_collar
    [0, 0.10, 0],           # 15 head
    [0.12, 0, 0],           # 16 left_shoulder
    [-0.12, 0, 0],          # 17 right_shoulder
    [0.25, 0, 0],           # 18 left_elbow
    [-0.25, 0, 0],          # 19 right_elbow
    [0.25, 0, 0],           # 20 left_wrist
    [-0.25, 0, 0],          # 21 right_wrist
    [0.08, 0, 0],           # 22 left_hand
    [-0.08, 0, 0],          # 23 right_hand
])


# ============================================================================
# SMPL 前向运动学
# ============================================================================

def rodrigues(r):
    """旋转向量(axis-angle)转旋转矩阵"""
    r = np.array(r).flatten()
    theta = np.linalg.norm(r)
    if theta < 1e-8:
        return np.eye(3)
    r = r / theta
    K = np.array([[0, -r[2], r[1]], [r[2], 0, -r[0]], [-r[1], r[0], 0]])
    return np.eye(3) + np.sin(theta) * K + (1 - np.cos(theta)) * np.dot(K, K)


def smpl_forward_kinematics(poses, Th, Rh):
    """
    SMPL 前向运动学 - 计算24个关节的世界坐标

    Args:
        poses: (69,) or (72,) - 每个关节的axis-angle旋转
        Th: (3,) - 全局平移 (相机坐标系)
        Rh: (3,) - 全局旋转 (axis-angle)

    Returns:
        joints: (24, 3) - 相机坐标系下的关节位置
    """
    poses = np.array(poses).flatten()
    Th = np.array(Th).flatten()
    Rh = np.array(Rh).flatten()

    if len(poses) == 69:
        joint_rotations = [np.eye(3)]
        for i in range(23):
            joint_rotations.append(rodrigues(poses[i * 3:(i + 1) * 3]))
    else:
        joint_rotations = []
        for i in range(24):
            joint_rotations.append(rodrigues(poses[i * 3:(i + 1) * 3]))

    R_global = rodrigues(Rh)
    global_transforms = [None] * 24
    joints = np.zeros((24, 3))

    for j in range(24):
        parent = SMPL_PARENTS[j]
        local_rot = joint_rotations[j]
        offset = SMPL_JOINT_OFFSETS[j]

        local_transform = np.eye(4)
        local_transform[:3, :3] = local_rot
        local_transform[:3, 3] = offset

        if parent == -1:
            global_transform = np.eye(4)
            global_transform[:3, :3] = R_global @ local_rot
            global_transform[:3, 3] = Th
            global_transforms[j] = global_transform
        else:
            parent_transform = global_transforms[parent]
            rotated_offset = parent_transform[:3, :3] @ offset
            global_transform = np.eye(4)
            global_transform[:3, :3] = parent_transform[:3, :3] @ local_rot
            global_transform[:3, 3] = parent_transform[:3, 3] + rotated_offset
            global_transforms[j] = global_transform

        joints[j] = global_transforms[j][:3, 3]

    return joints


# ============================================================================
# 数据加载
# ============================================================================

def load_pointcloud_aligned(npz_path):
    """
    加载 point_cloud_gen_aligned.py 生成的点云 NPZ 文件

    NPZ 格式:
        points: structured array (x, y, z, range, velocity, azimuth, elevation, power)
        frames: structured array (frame_id, timestamp, num_points, offset)

    Returns:
        list of dicts, 每帧含 frame_id, x, y, z, velocity, power, num_points
    """
    data = np.load(npz_path)
    frames_info = data['frames']
    all_points = data['points']

    print(f"点云文件: {npz_path}")
    print(f"  总帧数: {len(frames_info)}")
    print(f"  总点数: {len(all_points)}")

    frames_data = []
    for frame_info in frames_info:
        offset = frame_info['offset']
        num_points = frame_info['num_points']
        frame_points = all_points[offset:offset + num_points]

        frames_data.append({
            'frame_id': int(frame_info['frame_id']),
            'x': frame_points['x'].astype(np.float64),
            'y': frame_points['y'].astype(np.float64),
            'z': frame_points['z'].astype(np.float64),
            'velocity': frame_points['velocity'].astype(np.float64),
            'power': frame_points['power'].astype(np.float64),
            'num_points': int(num_points),
        })

    return frames_data


def radar_native_to_camera(pc_x, pc_y, pc_z, offset=(-0.15, 0.2, -0.03)):
    """
    将 point_cloud_gen_aligned.py 的雷达原生坐标转换到相机坐标系

    point_cloud_gen_aligned.py 坐标:
        pc_x = range * cos(el) * sin(az) → 对应 radar_Y (right)
        pc_y = range * cos(el) * cos(az) → 对应 radar_X (forward)
        pc_z = range * sin(el)           → 对应 radar_Z (up)

    相机坐标系 (与 export_pose_pointcloud.py radar_to_camera 一致):
        cam_x = -radar_Y = -pc_x  (镜像修正)
        cam_y = -radar_Z = -pc_z  (down)
        cam_z = radar_X  = pc_y   (forward/depth)
        + offset

    Args:
        pc_x, pc_y, pc_z: 雷达原生坐标
        offset: (ox, oy, oz) 相机坐标系下的偏移量

    Returns:
        cam_x, cam_y, cam_z: 相机坐标系坐标
    """
    cam_x = -pc_x + offset[0]
    cam_y = -pc_z + offset[1]
    cam_z = pc_y + offset[2]
    return cam_x, cam_y, cam_z


def load_smpl_data(smpl_folder):
    """加载 smpl_target 文件夹中的SMPL参数"""
    json_files = sorted(glob.glob(os.path.join(smpl_folder, '*.json')))

    if not json_files:
        print(f"警告: 在 {smpl_folder} 中未找到JSON文件")
        return None

    print(f"SMPL文件夹: {smpl_folder}")
    print(f"  总帧数: {len(json_files)}")

    smpl_data = []
    for json_file in json_files:
        with open(json_file, 'r') as f:
            data = json.load(f)
            if isinstance(data, list):
                data = data[0]
            smpl_data.append({
                'file': os.path.basename(json_file),
                'Rh': np.array(data['Rh']).flatten(),
                'Th': np.array(data['Th']).flatten(),
                'poses': np.array(data['poses']).flatten(),
                'shapes': np.array(data['shapes']).flatten(),
            })

    return smpl_data


def compute_all_smpl_joints(smpl_data):
    """
    预计算所有帧的SMPL关节位置 (相机坐标系)

    Returns:
        poses: (N, 24, 3) - 所有帧的关节位置
    """
    all_joints = []
    for frame in smpl_data:
        joints = smpl_forward_kinematics(frame['poses'], frame['Th'], frame['Rh'])
        all_joints.append(joints)
    return np.array(all_joints, dtype=np.float32)


# ============================================================================
# 可视化器
# ============================================================================

class AlignedVisualizer:
    def __init__(self, pc_frames, smpl_joints, camera_offset, fps=10):
        """
        Args:
            pc_frames: list of dicts (from load_pointcloud_aligned)
            smpl_joints: (N_smpl, 24, 3) 相机坐标系关节位置
            camera_offset: (3,) 雷达->相机坐标偏移
            fps: 播放帧率
        """
        self.pc_frames = pc_frames
        self.smpl_joints = smpl_joints
        self.camera_offset = camera_offset

        self.num_pc_frames = len(pc_frames)
        self.num_smpl_frames = len(smpl_joints) if smpl_joints is not None else 0
        self.num_frames = min(self.num_pc_frames, self.num_smpl_frames) if self.num_smpl_frames > 0 else self.num_pc_frames

        print(f"\n可视化帧数: {self.num_frames} (点云: {self.num_pc_frames}, SMPL: {self.num_smpl_frames})")

        # 预转换所有帧点云到相机坐标系
        print("转换点云坐标到相机坐标系...")
        self.cam_pointclouds = []
        for frame in self.pc_frames:
            if frame['num_points'] > 0:
                cx, cy, cz = radar_native_to_camera(
                    frame['x'], frame['y'], frame['z'], self.camera_offset
                )
                self.cam_pointclouds.append(np.column_stack([cx, cy, cz]))
            else:
                self.cam_pointclouds.append(np.zeros((0, 3)))

        # 状态
        self.current_frame_idx = 0
        self.aggregation_window = 1
        self.pointcloud_frame_offset = 0
        self.fps = fps
        self.playing = True
        self.loop = True
        self.show_pose = True
        self.show_pointcloud = True

        # 计算全局范围
        self._compute_ranges()

        # 默认视角 (与 vis_wicompass.py 一致)
        self.default_elev = -80
        self.default_azim = -90
        self.current_elev = self.default_elev
        self.current_azim = self.default_azim

        self.setup_figure()

    def _compute_ranges(self):
        """计算全局数据范围 (相机坐标系)"""
        # 收集所有点
        all_pc_points = [pc for pc in self.cam_pointclouds if len(pc) > 0]
        if all_pc_points:
            all_pc = np.vstack(all_pc_points)
            print(f"点云范围 (相机坐标系):")
            print(f"  X(横向): [{all_pc[:, 0].min():.3f}, {all_pc[:, 0].max():.3f}]")
            print(f"  Y(纵向): [{all_pc[:, 1].min():.3f}, {all_pc[:, 1].max():.3f}]")
            print(f"  Z(深度): [{all_pc[:, 2].min():.3f}, {all_pc[:, 2].max():.3f}]")
        else:
            all_pc = np.zeros((1, 3))

        if self.smpl_joints is not None and len(self.smpl_joints) > 0:
            all_joints = self.smpl_joints.reshape(-1, 3)
            print(f"骨架范围 (相机坐标系):")
            print(f"  X(横向): [{all_joints[:, 0].min():.3f}, {all_joints[:, 0].max():.3f}]")
            print(f"  Y(纵向): [{all_joints[:, 1].min():.3f}, {all_joints[:, 1].max():.3f}]")
            print(f"  Z(深度): [{all_joints[:, 2].min():.3f}, {all_joints[:, 2].max():.3f}]")
            all_data = np.vstack([all_pc, all_joints])
        else:
            all_data = all_pc

        self.x_min, self.x_max = all_data[:, 0].min(), all_data[:, 0].max()
        self.y_min, self.y_max = all_data[:, 1].min(), all_data[:, 1].max()
        self.z_min, self.z_max = all_data[:, 2].min(), all_data[:, 2].max()

    def setup_figure(self):
        """设置matplotlib图形"""
        plt.style.use('dark_background')
        self.fig = plt.figure(figsize=(14, 10))
        self.ax = self.fig.add_axes([0.05, 0.1, 0.85, 0.85], projection='3d')
        self.fig.canvas.manager.set_window_title('Aligned Point Cloud + SMPL Visualizer')

        self.ax.view_init(elev=self.default_elev, azim=self.default_azim)
        self.fig.canvas.mpl_connect('key_press_event', self.on_key_press)

        help_text = ('Space: Play/Pause | \u2190/\u2192: Frame | \u2191/\u2193: Speed | '
                     '1/3/5: Agg | [/]: Sync offset | 0: Reset offset | '
                     'P: Cloud | S: Skeleton | R: Reset view | L: Loop | Q: Quit')
        self.fig.text(0.5, 0.02, help_text, ha='center', fontsize=9, color='gray')
        self.title_text = self.fig.text(0.5, 0.95, '', ha='center', fontsize=12, color='white')

        self.scatter = None
        self.skeleton_lines = []
        self.joint_scatter = None

        self._setup_axes()

    def _setup_axes(self):
        """设置坐标轴"""
        self.ax.set_xlabel('X [m] (Horizontal)', fontsize=11, labelpad=10)
        self.ax.set_ylabel('Y [m] (Vertical)', fontsize=11, labelpad=10)
        self.ax.set_zlabel('Z [m] (Depth)', fontsize=11, labelpad=10)

        # 根据数据范围自动设置, 留一定余量 (缩小到1/3)
        # x_center = (self.x_min + self.x_max) / 2
        # y_center = (self.y_min + self.y_max) / 2
        # z_center = (self.z_min + self.z_max) / 2
        x_center = 0
        y_center = 0
        z_center = 3
        x_half = 1.5
        y_half = 1.5

        # x_half = max(slf.y_max - self.y_min) / 2, 0.5) * 0.25
        z_half = 1.5  # 固定Z轴显示范围为 [z_center-1.5, z_center+1.5]，即 3m 宽度

        self.ax.set_xlim(x_center - x_half, x_center + x_half)
        self.ax.set_ylim(y_center - y_half, y_center + y_half)
        self.ax.set_zlim(z_center - z_half, z_center + z_half)

    def filter_pointcloud_by_pelvis(self, pointcloud, pelvis_pos):
        """以骨盆位置为中心过滤点云"""
        if len(pointcloud) == 0:
            return pointcloud

        x_range = 1.5
        y_range = 1.5
        z_range = 1

        px, py, pz = pelvis_pos
        mask = (
            (np.abs(pointcloud[:, 0] - px) <= x_range) &
            (np.abs(pointcloud[:, 1] - py) <= y_range) &
            (np.abs(pointcloud[:, 2] - pz) <= z_range)
        )
        return pointcloud[mask]

    def get_aggregated_pointcloud(self):
        """获取聚合点云 (多帧叠加)"""
        half_window = (self.aggregation_window - 1) // 2
        center_idx = self.current_frame_idx + self.pointcloud_frame_offset
        center_idx = max(0, min(self.num_pc_frames - 1, center_idx))

        start_idx = max(0, center_idx - half_window)
        end_idx = min(self.num_pc_frames - 1, center_idx + half_window)

        # 获取当前骨盆位置用于过滤
        pelvis_pos = None
        if self.smpl_joints is not None and self.current_frame_idx < self.num_smpl_frames:
            pelvis_pos = self.smpl_joints[self.current_frame_idx, 0]  # joint 0 = pelvis

        points_list = []
        for i in range(start_idx, end_idx + 1):
            if i < len(self.cam_pointclouds):
                pc = self.cam_pointclouds[i]
                if len(pc) > 0:
                    if pelvis_pos is not None:
                        pc = self.filter_pointcloud_by_pelvis(pc, pelvis_pos)
                    if len(pc) > 0:
                        points_list.append(pc)

        if points_list:
            return np.vstack(points_list)
        else:
            return np.array([]).reshape(0, 3)

    def animate(self, frame_num):
        """动画回调"""
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
        """更新3D可视化"""
        self.current_elev = self.ax.elev
        self.current_azim = self.ax.azim

        self.ax.clear()

        # 当前骨架 (相机坐标系)
        pose = None
        if self.smpl_joints is not None and self.current_frame_idx < self.num_smpl_frames:
            pose = self.smpl_joints[self.current_frame_idx]  # (24, 3)

        # 当前聚合点云 (已转换到相机坐标系)
        pointcloud = self.get_aggregated_pointcloud()

        artists = []

        # 绘制点云
        if self.show_pointcloud and len(pointcloud) > 0:
            self.scatter = self.ax.scatter(
                pointcloud[:, 0], pointcloud[:, 1], pointcloud[:, 2],
                c=pointcloud[:, 2],
                cmap='viridis',
                s=10,
                alpha=0.6,
                vmin=self.z_min, vmax=self.z_max,
                label=f'Point Cloud ({len(pointcloud)} pts)'
            )
            artists.append(self.scatter)

        # 绘制骨架
        if self.show_pose and pose is not None:
            for start, end in SMPL_BONES:
                if start < len(pose) and end < len(pose):
                    xs = [pose[start, 0], pose[end, 0]]
                    ys = [pose[start, 1], pose[end, 1]]
                    zs = [pose[start, 2], pose[end, 2]]
                    line, = self.ax.plot(xs, ys, zs, 'r-', linewidth=2, alpha=0.9)
                    artists.append(line)

            self.joint_scatter = self.ax.scatter(
                pose[:, 0], pose[:, 1], pose[:, 2],
                c='red', s=30, alpha=1.0, marker='o',
                label='SMPL Joints (24)'
            )
            artists.append(self.joint_scatter)

        self._setup_axes()
        self.ax.view_init(elev=self.current_elev, azim=self.current_azim)

        if self.show_pose or self.show_pointcloud:
            self.ax.legend(loc='upper right', fontsize=9)

        # 标题
        frame_id = self.pc_frames[self.current_frame_idx]['frame_id']
        status = "\u25b6 Playing" if self.playing else "\u23f8 Paused"
        loop_status = "\U0001f501" if self.loop else ""
        pose_status = "\U0001f464" if self.show_pose else ""
        cloud_status = "\u2601\ufe0f" if self.show_pointcloud else ""

        pc_count = len(pointcloud) if self.show_pointcloud else 0
        offset_str = f"Sync: {self.pointcloud_frame_offset:+d}" if self.pointcloud_frame_offset != 0 else ""

        title = f'Frame {frame_id} ({self.current_frame_idx + 1}/{self.num_frames})'
        title += f'  |  Agg: {self.aggregation_window}  |  Points: {pc_count}'
        if offset_str:
            title += f'  |  {offset_str}'
        title += f'  |  {self.fps} FPS  |  {status} {loop_status} {pose_status} {cloud_status}'
        self.title_text.set_text(title)

        return artists

    def on_key_press(self, event):
        """键盘事件处理"""
        if event.key == ' ':
            self.playing = not self.playing
            print(f"Playback: {'Playing' if self.playing else 'Paused'}")
            if not self.playing:
                self.update_plot()
                self.fig.canvas.draw_idle()

        elif event.key in ['right', 'd', 'D']:
            self.playing = False
            self.current_frame_idx = min(self.current_frame_idx + 1, self.num_frames - 1)
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key in ['left', 'a', 'A']:
            self.playing = False
            self.current_frame_idx = max(self.current_frame_idx - 1, 0)
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key == 'up':
            self.fps = min(self.fps + 5, 60)
            print(f"FPS: {self.fps}")
            self.restart_animation()

        elif event.key == 'down':
            self.fps = max(self.fps - 5, 1)
            print(f"FPS: {self.fps}")
            self.restart_animation()

        elif event.key == '1':
            self.aggregation_window = 1
            print("Aggregation: Single frame")
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key == '3':
            self.aggregation_window = 3
            print("Aggregation: 3 frames (\u00b11)")
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key == '5':
            self.aggregation_window = 5
            print("Aggregation: 5 frames (\u00b12)")
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key in ['r', 'R']:
            self.current_elev = self.default_elev
            self.current_azim = self.default_azim
            self.ax.view_init(elev=self.default_elev, azim=self.default_azim)
            print("View reset")
            self.fig.canvas.draw_idle()

        elif event.key in ['p', 'P', 'c', 'C']:
            self.show_pointcloud = not self.show_pointcloud
            print(f"Point Cloud: {'ON' if self.show_pointcloud else 'OFF'}")
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key in ['s', 'S']:
            self.show_pose = not self.show_pose
            print(f"Skeleton: {'ON' if self.show_pose else 'OFF'}")
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key in ['l', 'L']:
            self.loop = not self.loop
            print(f"Loop: {'ON' if self.loop else 'OFF'}")

        elif event.key == '[':
            self.pointcloud_frame_offset -= 1
            print(f"PC offset: {self.pointcloud_frame_offset} frames")
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key == ']':
            self.pointcloud_frame_offset += 1
            print(f"PC offset: {self.pointcloud_frame_offset} frames")
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key == '0':
            self.pointcloud_frame_offset = 0
            print("PC offset reset to 0")
            self.update_plot()
            self.fig.canvas.draw_idle()

        elif event.key in ['q', 'Q', 'escape']:
            if hasattr(self, 'anim') and self.anim.event_source:
                self.anim.event_source.stop()
            plt.close(self.fig)
            print("Closed.")

    def restart_animation(self):
        """以新FPS重启动画"""
        if hasattr(self, 'anim') and self.anim.event_source:
            self.anim.event_source.stop()
        interval = 1000 // self.fps
        self.anim = FuncAnimation(
            self.fig, self.animate,
            interval=interval, blit=False, cache_frame_data=False
        )
        self.fig.canvas.draw_idle()

    def show(self):
        """显示可视化窗口"""
        print("\n" + "=" * 70)
        print("Aligned Point Cloud + SMPL Visualizer")
        print("=" * 70)
        print("Controls:")
        print("  Space        : Play/Pause")
        print("  Left/Right   : Previous/Next frame")
        print("  Up/Down      : Increase/Decrease FPS")
        print("  1/3/5        : Point cloud aggregation window")
        print("  [ / ]        : Adjust point cloud sync offset")
        print("  0            : Reset sync offset to 0")
        print("  P/C          : Toggle point cloud visibility")
        print("  S            : Toggle skeleton visibility")
        print("  R            : Reset view")
        print("  L            : Toggle loop")
        print("  Q/Escape     : Quit")
        print("  Mouse drag   : Rotate view")
        print(f"\nCamera offset: {self.camera_offset}")
        print("=" * 70 + "\n")

        interval = 1000 // self.fps
        self.anim = FuncAnimation(
            self.fig, self.animate,
            interval=interval, blit=False, cache_frame_data=False
        )
        plt.show()


# ============================================================================
# 主程序
# ============================================================================

def main():
    parser = argparse.ArgumentParser(
        description='Visualize aligned radar point cloud + SMPL skeleton',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument(
        '--pointcloud', type=str,
        default=r'D:\Codes\4.0.1\output_aligned\pointclouds\pointcloud_aligned.npz',
        help='Path to pointcloud_aligned.npz'
    )
    parser.add_argument(
        '--smpl-dir', type=str,
        default=r'D:\Codes\4.0.1\smpl_target',
        help='Path to SMPL JSON directory'
    )
    parser.add_argument('--fps', type=int, default=10, help='Playback FPS (default: 10)')
    parser.add_argument('--start-frame', type=int, default=0, help='Starting frame index')
    parser.add_argument(
        '--aggregation', type=int, choices=[1, 3, 5], default=3,
        help='Initial aggregation window (default: 3)'
    )
    parser.add_argument('--no-loop', action='store_true', help='Disable loop')
    parser.add_argument('--no-pose', action='store_true', help='Hide skeleton initially')
    parser.add_argument('--no-pointcloud', action='store_true', help='Hide point cloud initially')
    parser.add_argument(
        '--sync-offset', type=int, default=0,
        help='Point cloud frame offset for sync'
    )

    # 雷达-相机坐标偏移 (与 export_pose_pointcloud.py 默认参数一致)
    parser.add_argument('--offset-x', type=float, default=-0.15,
                        help='Camera X offset (default: -0.15)')
    parser.add_argument('--offset-y', type=float, default=0.2,
                        help='Camera Y offset (default: 0.2)')
    parser.add_argument('--offset-z', type=float, default=0.3,
                        help='Camera Z offset (default: -0.)')

    args = parser.parse_args()

    # 检查文件
    pc_path = Path(args.pointcloud)
    smpl_dir = Path(args.smpl_dir)

    if not pc_path.exists():
        print(f"Error: Point cloud file not found: {pc_path}")
        return
    if not smpl_dir.exists():
        print(f"Warning: SMPL directory not found: {smpl_dir}")

    camera_offset = (args.offset_x, args.offset_y, args.offset_z)

    print("=" * 70)
    print("加载数据...")
    print("=" * 70)
    print(f"点云: {pc_path}")
    print(f"SMPL: {smpl_dir}")
    print(f"坐标偏移 (camera): {camera_offset}")

    # 加载点云
    pc_frames = load_pointcloud_aligned(str(pc_path))

    # 加载SMPL
    smpl_joints = None
    if smpl_dir.exists():
        smpl_data = load_smpl_data(str(smpl_dir))
        if smpl_data:
            print("\n计算SMPL关节位置 (前向运动学)...")
            smpl_joints = compute_all_smpl_joints(smpl_data)
            print(f"  关节形状: {smpl_joints.shape}")

    # 创建可视化器
    visualizer = AlignedVisualizer(pc_frames, smpl_joints, camera_offset, fps=args.fps)

    # 设置初始状态
    if args.start_frame > 0:
        visualizer.current_frame_idx = min(args.start_frame, visualizer.num_frames - 1)
    visualizer.aggregation_window = args.aggregation
    visualizer.loop = not args.no_loop
    visualizer.show_pose = not args.no_pose
    visualizer.show_pointcloud = not args.no_pointcloud
    visualizer.pointcloud_frame_offset = args.sync_offset

    visualizer.show()


if __name__ == '__main__':
    main()
