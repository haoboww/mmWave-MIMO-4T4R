# SKY32B750_RC6011C MIMO 雷达处理系统

一套完整的基于 SKY32B750_RC6011C 4T4R MIMO 毫米波雷达的硬件驱动和数据处理系统。包含硬件控制代码、MATLAB 信号处理脚本和点云生成算法。

## 📁 项目结构

```
├── 1-SV4.0.1/              # 雷达硬件驱动代码 (C/C++)
│   ├── 硬件初始化
│   ├── USB 3.0 通信驱动
│   ├── 数据采集与缓存管理
│   └── 雷达参数配置接口
│
├── 3-matlab/               # MATLAB 信号处理脚本
│   ├── 天线校准数据 (*.mat)
│   ├── 信号处理算法
│   ├── 参数优化脚本
│   └── 可视化工具
│
├── 5-RADAR_V10/            # 低层硬件处理代码
│   ├── FFT 处理模块
│   ├── CFAR 检测算法
│   ├── 方位角/俯仰角估计
│   ├── Steering Vector 生成
│   └── 点云数据打包
│
├── point_cloud_gen.py              # Python 点云生成脚本 (主处理管道)
├── visualize_pointcloud_smpl.py    # 3D 可视化脚本 (点云 + 骨架显示)
├── requirements.txt                # Python 依赖配置
├── LICENSE                         # MIT 许可证
├── .gitignore                      # Git 忽略规则
└── README.md                       # 本文档
```

## 🔧 硬件配置

**雷达型号**: SKY32B750_RC6011C

**规格参数**:
- **天线阵列**: 4 TX × 4 RX (4T4R MIMO)
- **工作频率**: 77-81 GHz
- **接口**: USB 3.0 (高速数据传输)
- **采样方式**: TDM-MIMO (时分复用)

## 📊 数据处理流程

### point_cloud_gen.py 处理步骤

该脚本实现了完整的雷达信号处理管道，与硬件代码 `5-RADAR_V10` 完全对应:

1. **FFT1D (距离 FFT)**
   - 512 点 FFT
   - Hanning 窗函数
   - 距离分辨率: ~0.39 cm

2. **静态杂波抑制 (P2 Mean)**
   - Chirp 维度均值减法
   - 去除直流分量和固定目标

3. **FFT2D (多普勒 FFT)**
   - 256 点 FFT
   - TDM-MIMO 零填充处理
   - Hanning 窗函数
   - 多普勒分辨率: ~0.21 m/s (@ 77 GHz)

4. **CFAR 检测 (SO-CFAR)**
   - 所有天线幅度和上进行检测
   - 自适应阈值设置
   - 虚警概率 Pfa = 10^-6

5. **FFT3D (方位角 FFT)**
   - 128 点 FFT
   - 不加窗函数
   - 方位角分辨率: ~2.8°

6. **多峰检测 (DPK)**
   - Direction Peak Detection
   - 多目标分辨

7. **FFT4D (俯仰角 FFT)**
   - 128 点 FFT
   - Steering Vector 补偿
   - 俯仰角分辨率: ~2.8°

8. **点云生成与输出**
   - NPZ 格式存储
   - 包含距离、多普勒速度、方位角、俯仰角信息

## 🚀 快速开始

### 环境要求

**硬件**:
- Windows 10/11 系统
- USB 3.0 接口
- SKY32B750_RC6011C 雷达硬件

**软件**:
- Python 3.8+
- NumPy, SciPy
- Matplotlib
- MATLAB R2021a+ (可选，用于参数优化)

### Python 依赖安装

```bash
pip install numpy scipy matplotlib
```

### 运行点云生成

```bash
python point_cloud_gen.py
```

**输入**: 二进制原始雷达数据文件 (*.bin)
**输出**: 点云数据文件 (*.npz)

### 运行 3D 可视化

```bash
python visualize_pointcloud_smpl.py
```

**功能**: 交互式 3D 可视化工具，支持：
- 点云和 SMPL 骨架框架同步显示
- 帧序列实时播放
- 鼠标旋转/缩放视角
- 实时切换显示点云/骨架

**操作键**:
- `Space`: 播放/暂停
- `← →`: 上一帧/下一帧
- `↑ ↓`: 加速/减速
- `P`: 显示/隐藏点云
- `S`: 显示/隐藏骨架
- `1/3/5`: 聚合 N 帧点云
- `0`: 重置视角
- `Q`: 退出

## 📝 关键代码说明

### 可视化脚本 (visualize_pointcloud_smpl.py)

该脚本提供交互式 3D 可视化，可同时显示：
- **点云数据**: 雷达检测到的目标点
- **SMPL 骨架**: 人体姿态模型框架
- **帧同步**: 每帧点云对应一个 SMPL 状态

**主要特性**:
```python
# 支持多帧点云聚合
聚合_1帧    # 单帧点云
聚合_3帧    # 连续3帧点云合并
聚合_5帧    # 连续5帧点云合并

# 实时骨架显示
24个关节 + 23根骨骼 + SMPL 模型
关节名称: pelvis, left_hip, right_hip, spine, knee, ankle, 等

# 帧序列播放
自动循环播放
支持加速/减速播放
手动逐帧翻页
```

### 天线校准参数

系统内置了 16 路天线的幅度/相位校准参数:

```python
CALIB_AP_FLOAT = [
    1.000000000000000, 0.000000000000000,      # TX1/RX1
    -1.068481445312500, -0.019897460937500,   # TX1/RX2
    # ... 其他通道校准参数
]
```

这些参数来自硬件特征测量，用于补偿天线增益和相位失配。

### FFT 窗函数选择

不同处理阶段使用不同的窗函数：

| 阶段 | 窗函数 | 说明 |
|------|--------|------|
| FFT1D (Range) | Hanning | 降低旁瓣，提高检测性能 |
| FFT2D (Doppler) | Hanning | 保证频率分辨率 |
| FFT3D/4D (Angle) | 无窗 | 最大化角度分辨率 |

### CFAR 参数

```python
cell_size = 4          # CFAR 单元格大小
guard_size = 2         # 保护单元格数量
threshold_factor = 4   # 阈值倍数因子
```

## 📂 重要文件清单

### 根目录 Python 脚本

- **point_cloud_gen.py** - 主处理管道
  - FFT1D/2D/3D/4D 实现
  - CFAR 检测算法
  - 天线校准参数应用
  - 点云数据生成与保存

- **visualize_pointcloud_smpl.py** - 3D 交互式可视化
  - 点云渲染
  - SMPL 24 关节骨架显示
  - 帧同步显示
  - 多帧聚合功能

### 1-SV4.0.1 (硬件驱动)

- `App_Func.c/h` - 主应用函数，包含天线校准参数
- `App_USB3.c/h` - USB 3.0 通信实现
- `FX3_Firmware/` - FX3 固件文件
- `FPGA/` - FPGA 逻辑设计

### 3-matlab (信号处理)

- `*_calib.mat` - 频率/幅度/相位校准数据
- `*.m` - MATLAB 处理脚本和可视化工具
- `dataPrep60.m` - 数据预处理脚本
- `dataprocess.m` - 信号处理脚本

### 5-RADAR_V10 (低层算法)

- `Func02_Handle.c/h` - FFT 和信号处理核心算法
- `Func03_CFAR.c/h` - CFAR 检测实现
- `Func04_Angle.c/h` - 方位角/俯仰角估计

## 🔄 参数配置

### 雷达工作参数

主要配置参数位于硬件代码和 Python 脚本中:

```python
# 雷达参数
NUM_TX = 4                 # 发射天线数
NUM_RX = 4                 # 接收天线数
NUM_CHIRPS = 256           # 每帧 Chirp 数
NUM_ADC_SAMPLES = 512      # 每个 Chirp ADC 采样数
SAMPLING_RATE = 5.12e6     # ADC 采样率 (Hz)
CHIRP_BW = 4e9             # Chirp 带宽 (Hz)
```

### 处理参数微调

可在 `point_cloud_gen2.py` 中调整以下参数优化性能：

```python
# FFT 尺寸
FFT_SIZE_RANGE = 512       # 距离 FFT
FFT_SIZE_DOPPLER = 256     # 多普勒 FFT
FFT_SIZE_ANGLE = 128       # 角度 FFT

# CFAR 参数
CFAR_THRESHOLD = 4.0       # 检测阈值
CELL_SIZE = 4              # CFAR 单元格
GUARD_SIZE = 2             # 保护单元格
```

## 📊 输出数据格式

点云数据保存为 NPZ 格式，包含以下字段：

```python
{
    'range': np.array([...]),      # 距离 (m)
    'doppler': np.array([...]),    # 多普勒速度 (m/s)
    'azimuth': np.array([...]),    # 方位角 (度)
    'elevation': np.array([...]),  # 俯仰角 (度)
    'power': np.array([...])       # 检测功率 (线性)
}
```

## 🎯 应用场景

- ✓ 汽车自动驾驶雷达
- ✓ 手势识别系统
- ✓ 生命体征检测
- ✓ 无人机目标检测
- ✓ 室内定位和追踪


## ⚙️ 系统集成

### 硬件连接

1. 将 SKY32B750_RC6011C 通过 USB 3.0 连接到计算机
2. 安装 FX3 USB 驱动 (位于 `1-SV4.0.1/`)
3. 运行配置工具设置雷达参数

### 软件集成

```python
# 点云处理示例
from point_cloud_gen import RadarDataProcessor

# 初始化处理器
processor = RadarDataProcessor(
    calib_file='../3-matlab/calib.mat'
)

# 处理原始数据
point_cloud = processor.process(
    raw_data_file='data/frame_001.bin'
)

# 保存结果
processor.save_pointcloud(point_cloud, 'output.npz')

# 3D 可视化示例
from visualize_pointcloud_smpl import PointCloudVisualizer

visualizer = PointCloudVisualizer(
    pointcloud_dir='output/',
    smpl_dir='smpl_poses/'
)
visualizer.run()
```





