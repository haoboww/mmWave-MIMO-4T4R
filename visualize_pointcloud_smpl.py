"""
交互式3D可视化脚本 - 点云 + SMPL骨架 (帧同步版本)
每帧点云对应一个SMPL骨架，支持帧序列播放

使用方法:
    python visualize_pointcloud_smpl.py

操作说明:
    - 鼠标拖拽: 旋转3D视角
    - 滚轮: 缩放
    - 空格键: 播放/暂停
    - 左右方向键: 上一帧/下一帧
    - 上下方向键: 加速/减速
    - 1/3/5: 聚合1/3/5帧点云
    - P: 显示/隐藏点云
    - S: 显示/隐藏骨架
    - 0: 重置视角
    - Q: 退出
"""

import numpy as np
import json
import os
import glob
import matplotlib
matplotlib.use('TkAgg')  # 使用交互式后端
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.widgets import Slider, Button
import matplotlib.animation as animation


# SMPL 24关节骨架连接关系
SMPL_PARENTS = [-1, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 12, 13, 14, 16, 17, 18, 19, 20, 21]

# 关节名称
SMPL_JOINT_NAMES = [
    'pelvis', 'left_hip', 'right_hip', 'spine1', 'left_knee', 'right_knee',
    'spine2', 'left_ankle', 'right_ankle', 'spine3', 'left_foot', 'right_foot',
    'neck', 'left_collar', 'right_collar', 'head', 'left_shoulder', 'right_shoulder',
    'left_elbow', 'right_elbow', 'left_wrist', 'right_wrist', 'left_hand', 'right_hand'
]

# 骨骼连接 (用于绘制)
SMPL_BONES = [
    (0, 1), (0, 2), (0, 3),      # pelvis -> hips, spine
    (1, 4), (2, 5),              # hips -> knees
    (4, 7), (5, 8),              # knees -> ankles
    (7, 10), (8, 11),            # ankles -> feet
    (3, 6), (6, 9),              # spine chain
    (9, 12), (12, 15),           # spine -> neck -> head
    (9, 13), (9, 14),            # spine -> collars
    (13, 16), (14, 17),          # collars -> shoulders
    (16, 18), (17, 19),          # shoulders -> elbows
    (18, 20), (19, 21),          # elbows -> wrists
    (20, 22), (21, 23),          # wrists -> hands
]


def load_pointcloud(npz_path):
    """加载点云数据，按帧组织"""
    data = np.load(npz_path)
    frames_info = data['frames']
    all_points = data['points']
    
    print(f"点云文件: {npz_path}")
    print(f"  总帧数: {len(frames_info)}")
    print(f"  总点数: {len(all_points)}")
    
    # 按帧组织点云数据
    frames_data = []
    for i, frame_info in enumerate(frames_info):
        offset = frame_info['offset']
        num_points = frame_info['num_points']
        frame_points = all_points[offset:offset + num_points]
        
        frames_data.append({
            'frame_id': frame_info['frame_id'],
            'timestamp': frame_info['timestamp'],
            'x': frame_points['x'],
            'y': frame_points['y'],
            'z': frame_points['z'],
            'velocity': frame_points['velocity'],
            'power': frame_points['power'],
            'num_points': num_points
        })
    
    return frames_data


def load_smpl_data(smpl_folder):
    """加载SMPL数据"""
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
                'id': data.get('id', 0),
                'Rh': np.array(data['Rh']).flatten(),
                'Th': np.array(data['Th']).flatten(),
                'poses': np.array(data['poses']).flatten(),
                'shapes': np.array(data['shapes']).flatten(),
            })
    
    return smpl_data


def rodrigues(r):
    """旋转向量(axis-angle)转旋转矩阵 (Rodrigues公式)"""
    r = np.array(r).flatten()
    theta = np.linalg.norm(r)
    if theta < 1e-8:
        return np.eye(3)
    r = r / theta
    K = np.array([[0, -r[2], r[1]], [r[2], 0, -r[0]], [-r[1], r[0], 0]])
    return np.eye(3) + np.sin(theta) * K + (1 - np.cos(theta)) * np.dot(K, K)


# SMPL 骨骼的静态偏移 (从父关节到子关节的相对位置，单位：米)
# 这些是SMPL模型的近似值
SMPL_JOINT_OFFSETS = np.array([
    [0, 0, 0],              # 0 pelvis (root)
    [0.08, -0.06, 0],       # 1 left_hip (from pelvis)
    [-0.08, -0.06, 0],      # 2 right_hip (from pelvis)
    [0, 0.12, 0],           # 3 spine1 (from pelvis)
    [0, -0.40, 0],          # 4 left_knee (from left_hip)
    [0, -0.40, 0],          # 5 right_knee (from right_hip)
    [0, 0.12, 0],           # 6 spine2 (from spine1)
    [0, -0.42, 0],          # 7 left_ankle (from left_knee)
    [0, -0.42, 0],          # 8 right_ankle (from right_knee)
    [0, 0.12, 0],           # 9 spine3 (from spine2)
    [0, -0.08, 0.08],       # 10 left_foot (from left_ankle)
    [0, -0.08, 0.08],       # 11 right_foot (from right_ankle)
    [0, 0.12, 0],           # 12 neck (from spine3)
    [0.04, 0, 0],           # 13 left_collar (from spine3)
    [-0.04, 0, 0],          # 14 right_collar (from spine3)
    [0, 0.10, 0],           # 15 head (from neck)
    [0.12, 0, 0],           # 16 left_shoulder (from left_collar)
    [-0.12, 0, 0],          # 17 right_shoulder (from right_collar)
    [0.25, 0, 0],           # 18 left_elbow (from left_shoulder)
    [-0.25, 0, 0],          # 19 right_elbow (from right_shoulder)
    [0.25, 0, 0],           # 20 left_wrist (from left_elbow)
    [-0.25, 0, 0],          # 21 right_wrist (from right_elbow)
    [0.08, 0, 0],           # 22 left_hand (from left_wrist)
    [-0.08, 0, 0],          # 23 right_hand (from right_wrist)
])


def smpl_forward_kinematics(poses, Th, Rh):
    """
    SMPL 前向运动学 - 正确应用poses参数
    
    Args:
        poses: (69,) or (72,) 数组 - 每个关节的axis-angle旋转 (23或24个关节 × 3)
        Th: (3,) 数组 - 全局平移
        Rh: (3,) 数组 - 全局旋转 (axis-angle)
    
    Returns:
        joints: (24, 3) 数组 - 世界坐标系下的关节位置
    """
    poses = np.array(poses).flatten()
    Th = np.array(Th).flatten()
    Rh = np.array(Rh).flatten()
    
    # 处理poses长度 (可能是69或72)
    if len(poses) == 69:
        # 69维度: 不包含root旋转, 从joint 1开始
        joint_rotations = [np.eye(3)]  # root使用单位矩阵
        for i in range(23):
            rot_vec = poses[i*3:(i+1)*3]
            joint_rotations.append(rodrigues(rot_vec))
    else:
        # 72维度: 包含root旋转
        joint_rotations = []
        for i in range(24):
            rot_vec = poses[i*3:(i+1)*3]
            joint_rotations.append(rodrigues(rot_vec))
    
    # 全局旋转矩阵
    R_global = rodrigues(Rh)
    
    # 存储每个关节的全局变换矩阵 (4x4)
    global_transforms = [None] * 24
    
    # 存储关节位置
    joints = np.zeros((24, 3))
    
    # 计算每个关节的全局变换
    for j in range(24):
        parent = SMPL_PARENTS[j]
        local_rot = joint_rotations[j]
        offset = SMPL_JOINT_OFFSETS[j]
        
        # 构建局部变换矩阵
        local_transform = np.eye(4)
        local_transform[:3, :3] = local_rot
        local_transform[:3, 3] = offset
        
        if parent == -1:
            # Root关节: 应用全局旋转和平移
            global_transform = np.eye(4)
            global_transform[:3, :3] = R_global @ local_rot
            global_transform[:3, 3] = Th
            global_transforms[j] = global_transform
        else:
            # 子关节: 相对于父关节
            parent_transform = global_transforms[parent]
            # 旋转偏移量并加上父关节位置
            rotated_offset = parent_transform[:3, :3] @ offset
            
            global_transform = np.eye(4)
            global_transform[:3, :3] = parent_transform[:3, :3] @ local_rot
            global_transform[:3, 3] = parent_transform[:3, 3] + rotated_offset
            global_transforms[j] = global_transform
        
        joints[j] = global_transforms[j][:3, 3]
    
    return joints


class PointCloudSMPLVisualizer:
    """点云+SMPL骨架交互式可视化器"""
    
    def __init__(self, pointcloud_frames, smpl_data, frame_offset=0):
        self.pc_frames = pointcloud_frames
        self.smpl_data = smpl_data
        self.frame_offset = frame_offset  # SMPL帧相对于点云帧的偏移
        
        self.num_pc_frames = len(pointcloud_frames)
        self.num_smpl_frames = len(smpl_data) if smpl_data else 0
        self.num_frames = min(self.num_pc_frames, self.num_smpl_frames) if smpl_data else self.num_pc_frames
        
        print(f"\n可视化帧数: {self.num_frames} (点云:{self.num_pc_frames}, SMPL:{self.num_smpl_frames})")
        
        # 点云偏移量 (用于微调对齐)
        self.pc_offset = [0.0, 0.0, 0.0]
        
        # 骨架偏移量 (用于对齐骨架和点云)
        self.skeleton_offset = [0.0, 0.0, 0.0]
        
        # 是否将点云转换到相机坐标系 (默认关闭，改为转换骨架)
        self.transform_to_camera = False
        
        # 是否将骨架从相机坐标系转换到雷达坐标系（默认开启）
        self.transform_skeleton_to_radar = True
        
        # 以骨架为中心的显示范围
        self.focus_on_skeleton = True
        
        # 分析数据范围 (必须在上述参数初始化之后)
        self._analyze_data_range()
        
        self.current_frame = 0
        self.playing = False
        self.play_speed = 100  # ms per frame
        self.aggregate_frames = 1  # 聚合帧数
        self.show_pointcloud = True
        self.show_skeleton = True
        
        self.setup_figure()
        self.setup_controls()
        self.update_view()
    
    def _transform_pointcloud_to_camera(self, x, y, z):
        """
        将点云从雷达坐标系转换到相机坐标系
        
        雷达坐标系 (来自 mmwave_radar_collector.py):
            X = forward (boresight, 雷达正前方)
            Y = right (lateral, 雷达右侧)  
            Z = up (vertical, 垂直向上)
        
        相机坐标系 (SMPL/EasyMocap):
            X = right (图像右侧)
            Y = down (图像下方)
            Z = forward (相机正前方/深度方向)
        
        转换 (与 export_mmwave_dataset.py 一致):
            cam_X = -radar_Y (右侧，但需要镜像修正)
            cam_Y = -radar_Z (下方 = -上方)
            cam_Z = radar_X (前方)
        """
        cam_x = -y  # cam_X = -radar_Y (镜像修正)
        cam_y = -z  # cam_Y = -radar_Z
        cam_z = x   # cam_Z = radar_X
        return cam_x, cam_y, cam_z
    
    def _apply_pointcloud_offset(self, x, y, z):
        """应用点云偏移量 (用于微调对齐)"""
        return (x + self.pc_offset[0], 
                y + self.pc_offset[1], 
                z + self.pc_offset[2])
    
    def _transform_skeleton_to_radar(self, joints):
        """
        将骨架从相机坐标系转换到雷达坐标系
        
        相机坐标系 (SMPL/EasyMocap):
            X = right (图像右侧)
            Y = down (图像下方)
            Z = forward (相机正前方/深度方向)
        
        雷达坐标系:
            X = forward (boresight, 雷达正前方)
            Y = right (lateral, 雷达右侧)  
            Z = up (vertical, 垂直向上)
        
        转换 (相机->雷达):
            radar_X = cam_Z (前方)
            radar_Y = -cam_X (右侧，镜像修正)
            radar_Z = -cam_Y (上方 = -下方)
        """
        transformed = np.zeros_like(joints)
        transformed[:, 0] = joints[:, 2]   # radar_X = cam_Z
        transformed[:, 1] = -joints[:, 0]  # radar_Y = -cam_X
        transformed[:, 2] = -joints[:, 1]  # radar_Z = -cam_Y
        return transformed
    
    def _apply_skeleton_offset(self, joints):
        """应用骨架偏移量"""
        offset_joints = joints.copy()
        offset_joints[:, 0] += self.skeleton_offset[0]
        offset_joints[:, 1] += self.skeleton_offset[1]
        offset_joints[:, 2] += self.skeleton_offset[2]
        return offset_joints
    
    def _analyze_data_range(self):
        """分析点云和SMPL数据的范围"""
        # 点云数据范围 (雷达坐标系)
        all_x, all_y, all_z = [], [], []
        for frame in self.pc_frames[:min(10, len(self.pc_frames))]:  # 取前10帧分析
            all_x.extend(frame['x'])
            all_y.extend(frame['y'])
            all_z.extend(frame['z'])
        
        if len(all_x) > 0:
            all_x = np.array(all_x)
            all_y = np.array(all_y)
            all_z = np.array(all_z)
            print(f"\n点云数据范围 (雷达坐标系, 前10帧):")
            print(f"  X(前): [{all_x.min():.2f}, {all_x.max():.2f}]")
            print(f"  Y(右): [{all_y.min():.2f}, {all_y.max():.2f}]")
            print(f"  Z(上): [{all_z.min():.2f}, {all_z.max():.2f}]")
            
            # 点云使用雷达坐标系
            self.pc_center = [np.mean(all_x), np.mean(all_y), np.mean(all_z)]
            self.pc_range = max(all_x.max()-all_x.min(), all_y.max()-all_y.min(), all_z.max()-all_z.min())
        else:
            self.pc_center = [2, 0, 0]
            self.pc_range = 3
        
        # 默认骨架范围（防止没有骨架数据时出错）
        self.skeleton_center = [2, 0, 0]
        self.skeleton_range_x = 2.0
        self.skeleton_range_y = 2.0
        self.skeleton_range_z = 2.0
        
        # SMPL骨架数据范围
        if self.smpl_data:
            # 计算前10帧的所有关节位置
            all_joints_cam = []
            all_joints_radar = []
            for smpl_frame in self.smpl_data[:min(10, len(self.smpl_data))]:
                joints = smpl_forward_kinematics(
                    smpl_frame['poses'],
                    smpl_frame['Th'],
                    smpl_frame['Rh']
                )
                all_joints_cam.extend(joints)
                # 转换到雷达坐标系
                joints_radar = self._transform_skeleton_to_radar(joints)
                all_joints_radar.extend(joints_radar)
            
            all_joints_cam = np.array(all_joints_cam)
            all_joints_radar = np.array(all_joints_radar)
            
            print(f"\n骨架数据范围 (相机坐标系, 原始):")
            print(f"  X(右): [{all_joints_cam[:, 0].min():.2f}, {all_joints_cam[:, 0].max():.2f}]")
            print(f"  Y(下): [{all_joints_cam[:, 1].min():.2f}, {all_joints_cam[:, 1].max():.2f}]")
            print(f"  Z(前): [{all_joints_cam[:, 2].min():.2f}, {all_joints_cam[:, 2].max():.2f}]")
            
            print(f"\n骨架数据范围 (雷达坐标系, 转换后):")
            print(f"  X(前): [{all_joints_radar[:, 0].min():.2f}, {all_joints_radar[:, 0].max():.2f}]")
            print(f"  Y(右): [{all_joints_radar[:, 1].min():.2f}, {all_joints_radar[:, 1].max():.2f}]")
            print(f"  Z(上): [{all_joints_radar[:, 2].min():.2f}, {all_joints_radar[:, 2].max():.2f}]")
            
            # 根据是否转换，选择用于显示范围计算的数据
            if self.transform_skeleton_to_radar:
                poses_for_display = all_joints_radar
                print(f"\n使用: 坐标转换后的骨架数据")
            else:
                poses_for_display = all_joints_cam
                print(f"\n使用: 原始骨架数据")
            
            # 以骨架为中心设置显示范围 (扩大到约两倍)
            self.skeleton_center = [np.mean(poses_for_display[:, i]) for i in range(3)]
            self.skeleton_range_x = poses_for_display[:, 0].max() - poses_for_display[:, 0].min()
            self.skeleton_range_y = poses_for_display[:, 1].max() - poses_for_display[:, 1].min()
            self.skeleton_range_z = poses_for_display[:, 2].max() - poses_for_display[:, 2].min()
            
            if self.focus_on_skeleton:
                self.pc_center = self.skeleton_center
                self.pc_range = max(self.skeleton_range_x, self.skeleton_range_y, self.skeleton_range_z) * 2.4
            
            print(f"\n点云偏移量: {self.pc_offset}")
            print(f"骨架偏移量: {self.skeleton_offset}")
    
    def setup_figure(self):
        """设置图形窗口"""
        self.fig = plt.figure(figsize=(14, 10), facecolor='black')
        self.fig.canvas.manager.set_window_title('点云 + SMPL骨架 可视化')
        
        # 3D轴
        self.ax = self.fig.add_subplot(111, projection='3d', facecolor='#1a1a1a')
        self.ax.set_position([0.05, 0.15, 0.9, 0.8])
        
        # 设置轴样式
        self.ax.set_xlabel('X (m)', color='white', fontsize=10)
        self.ax.set_ylabel('Y (m)', color='white', fontsize=10)
        self.ax.set_zlabel('Z (m)', color='white', fontsize=10)
        self.ax.tick_params(colors='white')
        self.ax.xaxis.pane.fill = False
        self.ax.yaxis.pane.fill = False
        self.ax.zaxis.pane.fill = False
        self.ax.xaxis.pane.set_edgecolor('gray')
        self.ax.yaxis.pane.set_edgecolor('gray')
        self.ax.zaxis.pane.set_edgecolor('gray')
        self.ax.grid(True, alpha=0.3)
        
        # 以骨架为中心，设置不同轴的显示范围（扩大到原来的2-3倍）
        if self.focus_on_skeleton:
            margin_x = max(self.skeleton_range_x * 2.4, 3.2)
            margin_y = max(self.skeleton_range_y * 2.4, 2.4)
            margin_z = max(self.skeleton_range_z * 2.0, 2.4)
            
            self.ax.set_xlim(self.pc_center[0] - margin_x, self.pc_center[0] + margin_x)
            self.ax.set_ylim(self.pc_center[1] - margin_y, self.pc_center[1] + margin_y)
            self.ax.set_zlim(self.pc_center[2] - margin_z, self.pc_center[2] + margin_z)
            
            # 设置坐标轴视觉比例，让人物显示更真实
            x_range = 2 * margin_x
            y_range = 2 * margin_y
            z_range = 2 * margin_z
            self.ax.set_box_aspect([x_range, y_range, z_range * 1.5])
        else:
            margin = self.pc_range * 0.6
            self.ax.set_xlim(self.pc_center[0] - margin, self.pc_center[0] + margin)
            self.ax.set_ylim(self.pc_center[1] - margin, self.pc_center[1] + margin)
            self.ax.set_zlim(self.pc_center[2] - margin, self.pc_center[2] + margin)
        
        # 标题
        self.title = self.ax.set_title('', color='white', fontsize=12, pad=10)
        
        # 初始化绑定
        self.scatter = None
        self.skeleton_lines = []
        self.joint_scatter = None
    
    def setup_controls(self):
        """设置控件和键盘事件"""
        # 帧滑块
        ax_slider = self.fig.add_axes([0.15, 0.05, 0.65, 0.03], facecolor='#333333')
        self.frame_slider = Slider(ax_slider, 'Frame', 0, self.num_frames - 1, 
                                   valinit=0, valstep=1, color='#4CAF50')
        self.frame_slider.label.set_color('white')
        self.frame_slider.valtext.set_color('white')
        self.frame_slider.on_changed(self.on_slider_change)
        
        # 播放按钮
        ax_play = self.fig.add_axes([0.85, 0.05, 0.08, 0.03])
        self.play_btn = Button(ax_play, '▶ Play', color='#333333', hovercolor='#555555')
        self.play_btn.label.set_color('white')
        self.play_btn.on_clicked(self.toggle_play)
        
        # 键盘事件
        self.fig.canvas.mpl_connect('key_press_event', self.on_key_press)
        
        # 帮助文本
        help_text = ('Space: Play/Pause | ←/→: Frame | ↑/↓: Speed | 1/3/5: Agg | [/]: Sync | T: Skel Transform | '
                    'x/X y/Y z/Z: PC Offset | j/J k/K l/L: Skel Offset | R/B: Reset | P: Cloud | S: Skel | Q: Quit')
        self.fig.text(0.5, 0.01, help_text, ha='center', va='bottom', 
                     color='gray', fontsize=8)
    
    def on_slider_change(self, val):
        """滑块变化回调"""
        self.current_frame = int(val)
        self.update_view()
    
    def toggle_play(self, event=None):
        """切换播放状态"""
        self.playing = not self.playing
        self.play_btn.label.set_text('⏸ Pause' if self.playing else '▶ Play')
        if self.playing:
            self.animate()
    
    def animate(self):
        """动画循环"""
        if not self.playing:
            return
        
        self.current_frame = (self.current_frame + 1) % self.num_frames
        self.frame_slider.set_val(self.current_frame)
        
        # 使用after调度下一帧
        self.fig.canvas.draw_idle()
        self.fig.canvas.start_event_loop(self.play_speed / 1000.0)
        
        if self.playing:
            self.fig.canvas.start_event_loop(0.001)
            self.animate()
    
    def on_key_press(self, event):
        """键盘事件处理"""
        if event.key == ' ':
            self.toggle_play()
        elif event.key == 'right':
            self.current_frame = (self.current_frame + 1) % self.num_frames
            self.frame_slider.set_val(self.current_frame)
        elif event.key == 'left':
            self.current_frame = (self.current_frame - 1) % self.num_frames
            self.frame_slider.set_val(self.current_frame)
        elif event.key == 'up':
            self.play_speed = max(20, self.play_speed - 20)
            print(f"播放速度: {1000/self.play_speed:.1f} fps")
        elif event.key == 'down':
            self.play_speed = min(500, self.play_speed + 20)
            print(f"播放速度: {1000/self.play_speed:.1f} fps")
        elif event.key in ['1', '3', '5']:
            self.aggregate_frames = int(event.key)
            print(f"聚合帧数: {self.aggregate_frames}")
            self.update_view()
        elif event.key == 'p' or event.key == 'c':
            self.show_pointcloud = not self.show_pointcloud
            print(f"点云显示: {'开' if self.show_pointcloud else '关'}")
            self.update_view()
        elif event.key == 's':
            self.show_skeleton = not self.show_skeleton
            print(f"骨架显示: {'开' if self.show_skeleton else '关'}")
            self.update_view()
        elif event.key == '[':
            self.frame_offset -= 1
            print(f"SMPL帧偏移: {self.frame_offset}")
            self.update_view()
        elif event.key == ']':
            self.frame_offset += 1
            print(f"SMPL帧偏移: {self.frame_offset}")
            self.update_view()
        elif event.key == '0':
            self.frame_offset = 0
            print(f"SMPL帧偏移已重置: {self.frame_offset}")
            self.update_view()
        elif event.key == 'q':
            plt.close(self.fig)
        elif event.key == 't':
            # 切换骨架坐标系转换
            self.transform_skeleton_to_radar = not self.transform_skeleton_to_radar
            print(f"骨架坐标系转换: {'相机->雷达' if self.transform_skeleton_to_radar else '原始(相机)'}")
            self.update_view()
        elif event.key == 'x':
            # 调整X偏移
            self.pc_offset[0] += 0.05
            print(f"点云偏移: {self.pc_offset}")
            self.update_view()
        elif event.key == 'X':
            self.pc_offset[0] -= 0.05
            print(f"点云偏移: {self.pc_offset}")
            self.update_view()
        elif event.key == 'y':
            self.pc_offset[1] += 0.05
            print(f"点云偏移: {self.pc_offset}")
            self.update_view()
        elif event.key == 'Y':
            self.pc_offset[1] -= 0.05
            print(f"点云偏移: {self.pc_offset}")
            self.update_view()
        elif event.key == 'z':
            self.pc_offset[2] += 0.05
            print(f"点云偏移: {self.pc_offset}")
            self.update_view()
        elif event.key == 'Z':
            self.pc_offset[2] -= 0.05
            print(f"点云偏移: {self.pc_offset}")
            self.update_view()
        elif event.key == 'r':
            # 重置点云偏移
            self.pc_offset = [0.0, 0.0, 0.0]
            print(f"点云偏移已重置: {self.pc_offset}")
            self.update_view()
        # 骨架偏移调整 (j/J, k/K, l/L)
        elif event.key == 'j':
            self.skeleton_offset[0] += 0.05
            print(f"骨架偏移: {self.skeleton_offset}")
            self.update_view()
        elif event.key == 'J':
            self.skeleton_offset[0] -= 0.05
            print(f"骨架偏移: {self.skeleton_offset}")
            self.update_view()
        elif event.key == 'k':
            self.skeleton_offset[1] += 0.05
            print(f"骨架偏移: {self.skeleton_offset}")
            self.update_view()
        elif event.key == 'K':
            self.skeleton_offset[1] -= 0.05
            print(f"骨架偏移: {self.skeleton_offset}")
            self.update_view()
        elif event.key == 'l':
            self.skeleton_offset[2] += 0.05
            print(f"骨架偏移: {self.skeleton_offset}")
            self.update_view()
        elif event.key == 'L':
            self.skeleton_offset[2] -= 0.05
            print(f"骨架偏移: {self.skeleton_offset}")
            self.update_view()
        elif event.key == 'b':
            self.skeleton_offset = [0.0, 0.0, 0.0]
            print(f"骨架偏移已重置: {self.skeleton_offset}")
            self.update_view()
    
    def update_view(self):
        """更新视图"""
        # 清除旧元素
        if self.scatter:
            self.scatter.remove()
            self.scatter = None
        for line in self.skeleton_lines:
            line.remove()
        self.skeleton_lines = []
        if self.joint_scatter:
            self.joint_scatter.remove()
            self.joint_scatter = None
        
        # 获取当前帧数据
        pc_frame_idx = self.current_frame
        smpl_frame_idx = (self.current_frame + self.frame_offset) % self.num_smpl_frames if self.smpl_data else -1
        
        # 聚合多帧点云
        x_all, y_all, z_all, v_all = [], [], [], []
        for i in range(self.aggregate_frames):
            idx = (pc_frame_idx - i) % self.num_pc_frames
            if idx >= 0 and idx < self.num_pc_frames:
                frame = self.pc_frames[idx]
                x_all.extend(frame['x'])
                y_all.extend(frame['y'])
                z_all.extend(frame['z'])
                v_all.extend(frame['velocity'])
        
        x_all = np.array(x_all)
        y_all = np.array(y_all)
        z_all = np.array(z_all)
        v_all = np.array(v_all)
        
        # 点云保持雷达坐标系，只应用偏移
        if len(x_all) > 0:
            x_all, y_all, z_all = self._apply_pointcloud_offset(x_all, y_all, z_all)
        
        # 绘制点云
        if self.show_pointcloud and len(x_all) > 0:
            # 颜色映射 (基于速度)
            colors = plt.cm.viridis((v_all - v_all.min()) / (v_all.max() - v_all.min() + 1e-6))
            self.scatter = self.ax.scatter(x_all, y_all, z_all, 
                                          c=colors, s=15, alpha=0.8)
        
        # 绘制SMPL骨架
        if self.show_skeleton and self.smpl_data and smpl_frame_idx >= 0:
            smpl_frame = self.smpl_data[smpl_frame_idx]
            joints = smpl_forward_kinematics(
                smpl_frame['poses'],
                smpl_frame['Th'],
                smpl_frame['Rh']
            )
            # 将骨架从相机坐标系转换到雷达坐标系
            if self.transform_skeleton_to_radar:
                joints = self._transform_skeleton_to_radar(joints)
            # 应用骨架偏移
            joints = self._apply_skeleton_offset(joints)
            
            # 绘制骨骼线
            for start, end in SMPL_BONES:
                line, = self.ax.plot(
                    [joints[start, 0], joints[end, 0]],
                    [joints[start, 1], joints[end, 1]],
                    [joints[start, 2], joints[end, 2]],
                    'r-', linewidth=3, alpha=0.9
                )
                self.skeleton_lines.append(line)
            
            # 绘制关节点
            self.joint_scatter = self.ax.scatter(
                joints[:, 0], joints[:, 1], joints[:, 2],
                c='red', s=40, marker='o', alpha=1.0
            )
        
        # 更新标题
        pc_info = self.pc_frames[pc_frame_idx]
        title = f'Frame: {self.current_frame}/{self.num_frames-1} | '
        title += f'PC Points: {pc_info["num_points"]} | '
        if self.smpl_data and smpl_frame_idx >= 0:
            title += f'SMPL: {self.smpl_data[smpl_frame_idx]["file"]} | '
        title += f'Agg: {self.aggregate_frames} | Speed: {1000/self.play_speed:.0f}fps'
        self.title.set_text(title)
        
        self.fig.canvas.draw_idle()
    
    def show(self):
        """显示窗口"""
        plt.show()


def main():
    """主函数"""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    pointcloud_path = os.path.join(script_dir, 'pointcloud_output.npz')
    smpl_folder = os.path.join(script_dir, 'smpl_target')
    
    print("=" * 60)
    print("点云 + SMPL骨架 交互式可视化")
    print("=" * 60)
    
    # 加载数据
    pointcloud_frames = load_pointcloud(pointcloud_path)
    smpl_data = load_smpl_data(smpl_folder)
    
    print("\n" + "=" * 60)
    print("操作说明:")
    print("  鼠标拖拽: 旋转3D视角")
    print("  滚轮: 缩放")
    print("  空格键: 播放/暂停")
    print("  左/右方向键: 上一帧/下一帧")
    print("  上/下方向键: 加速/减速")
    print("  1/3/5: 聚合1/3/5帧点云")
    print("  P/C: 显示/隐藏点云")
    print("  S: 显示/隐藏骨架")
    print("  T: 切换骨架坐标转换 (相机->雷达)")
    print("  --- 点云偏移 ---")
    print("  x/X, y/Y, z/Z: 调整点云偏移 (+/-0.05m)")
    print("  R: 重置点云偏移")
    print("  --- 骨架偏移 ---")
    print("  j/J, k/K, l/L: 调整骨架X/Y/Z偏移 (+/-0.05m)")
    print("  B: 重置骨架偏移")
    print("  --- 帧同步 ---")
    print("  [/]: 调整SMPL帧偏移")
    print("  0: 重置SMPL帧偏移")
    print("  Q: 退出")
    print("=" * 60)
    
    # 创建可视化器
    visualizer = PointCloudSMPLVisualizer(pointcloud_frames, smpl_data)
    visualizer.show()


if __name__ == '__main__':
    main()
