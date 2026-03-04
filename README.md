# MIMO 毫米波雷达处理系统

一套完整的 MIMO 毫米波雷达数据处理系统，支持两种雷达平台：
- **SKY32B750_RC6011C** — 4T4R MIMO (77-81 GHz)
- **BGT60ATR24C** — 2T4R MIMO (60 GHz)

包含硬件控制代码、MATLAB 信号处理脚本、Python 点云生成和 3D 可视化工具。

## 📁 项目结构

```
├── 1-SV4.0.1/                          # 4T4R 雷达硬件驱动代码 (C/C++)
│   ├── 硬件初始化
│   ├── USB 3.0 通信驱动
│   ├── 数据采集与缓存管理
│   └── 雷达参数配置接口
│
├── 3-matlab/                           # MATLAB 信号处理脚本
│   ├── 天线校准数据 (*.mat)
│   ├── 信号处理算法
│   ├── 参数优化脚本
│   └── 可视化工具
│
├── 5-RADAR_V10/                        # 4T4R 低层硬件处理代码
│   ├── FFT 处理模块
│   ├── CFAR 检测算法
│   ├── 方位角/俯仰角估计
│   ├── Steering Vector 生成
│   └── 点云数据打包
│
├── Radar_2T4R/                         # BGT60ATR24C 2T4R 雷达处理
│   ├── point_cloud_gen_2t4r.py         #   点云生成 (主处理管道)
│   ├── visualize_pointcloud_2t4r.py    #   3D 交互式可视化
│   └── rdmap_2t4r.py                   #   Range-Doppler Map 生成
│
├── point_cloud_gen.py                  # 4T4R 点云生成 (主处理管道)
├── visualize_pointcloud_smpl.py        # 4T4R 3D 可视化 (点云 + SMPL 骨架)
├── requirements.txt                    # Python 依赖配置
├── LICENSE                             # MIT 许可证
├── .gitignore                          # Git 忽略规则
└── README.md                           # 本文档
```

---

## 🔧 支持的雷达平台

### 1. SKY32B750_RC6011C (4T4R)

| 参数 | 值 |
|------|------|
| 天线阵列 | 4TX × 4RX (16 虚拟天线) |
| 工作频率 | 58.5-62.3 GHz |
| 带宽 | 3.8 GHz |
| 采样方式 | TDM-MIMO |
| FFT1D | 512 点 (Range) |
| FFT2D | 256 点 (Doppler) |
| FFT3D | 128 点 (方位角, 8 天线) |
| FFT4D | 128 点 (俯仰角, 3 天线 + Steering Vector) |
| 距离分辨率 | ~3.9 cm |
| 角度分辨率 | ~0.9° |

### 2. BGT60ATR24C (2T4R)

| 参数 | 值 |
|------|------|
| 天线阵列 | 2TX × 4RX (8 虚拟天线) |
| 工作频率 | 60 GHz (中心 59.5 GHz) |
| 带宽 | 3 GHz |
| ADC 采样率 | 1 MHz |
| 每 chirp 采样 | 256 点 |
| 每帧 chirp 数 | 128 |
| FFT1D | 512 点 (Range, 2x 零填充) |
| FFT2D | 256 点 (Doppler, 2x 零填充) |
| 角度 FFT | 64 点 (方位角) |
| 距离分辨率 | 5.0 cm |
| 最大距离 | 6.4 m |
| 速度分辨率 | 0.020 m/s |
| 最大速度 | ±2.52 m/s |
| 角度分辨率 | ~1.8° |

---

## 📊 数据处理流程

### 4T4R: point_cloud_gen.py

与硬件代码 `5-RADAR_V10` 完全对应的 7 步处理管道：

1. **FFT1D** — 512 点 Range FFT + Hanning 窗
2. **P2 Mean** — Chirp 维度均值减法 (静态杂波抑制)
3. **FFT2D** — 256 点 Doppler FFT + TDM-MIMO 零填充 + Hanning 窗
4. **CFAR** — SO-CFAR 检测 (16 天线幅度和)
5. **FFT3D** — 128 点方位角 FFT (8 天线, 无窗)
6. **DPK** — sinc 插值多峰检测
7. **FFT4D** — 128 点俯仰角 FFT (Steering Vector)

### 2T4R: Radar_2T4R/point_cloud_gen_2t4r.py

针对 BGT60ATR24C 的 6 步处理管道：

1. **Range FFT** — Blackman-Harris 窗 + 512 点 FFT (2x 零填充)
2. **Mean Subtract** — 前帧/当前帧均值减法 (去除静态杂波)
3. **Doppler FFT** — Blackman-Harris 窗 + 256 点 FFT (2x 零填充)
4. **ABS SUM** — 8 天线幅度求和
5. **SO-CFAR** — 速度维度 CFAR 检测 + 局部极大值
6. **Angle FFT** — 64 点方位角 FFT (8 虚拟天线)

---

## 🚀 快速开始

### 环境要求

- Python 3.8+
- Windows 10/11

### 安装依赖

```bash
pip install numpy scipy matplotlib
```

### 4T4R 处理

```bash
# 点云生成
python point_cloud_gen.py

# 3D 可视化 (点云 + SMPL 骨架)
python visualize_pointcloud_smpl.py
```

### 2T4R 处理

```bash
cd Radar_2T4R

# Range-Doppler Map 查看
python rdmap_2t4r.py

# 点云生成
python point_cloud_gen_2t4r.py

# 3D 交互式可视化
python visualize_pointcloud_2t4r.py
```

---

## 🎮 可视化操作

两个可视化脚本均支持交互操作：

| 按键 | 功能 |
|------|------|
| `Space` | 播放/暂停 |
| `← →` 或 `A/D` | 上一帧/下一帧 |
| `↑ ↓` | 加速/减速 |
| `1/3/5` | 聚合 1/3/5 帧点云 |
| `T` | 切换颜色模式 (速度/功率) |
| `P` | 显示/隐藏点云 |
| `S` | 显示/隐藏骨架 (仅 4T4R) |
| `R` | 重置视角 |
| `L` | 切换循环播放 |
| `Q/Escape` | 退出 |

---

## 📊 输出数据格式

点云数据统一保存为 NPZ 格式：

```python
# points: structured array
#   x, y, z        — 笛卡尔坐标 (m)
#   range           — 距离 (m)
#   velocity        — 多普勒速度 (m/s)
#   azimuth         — 方位角 (度)
#   elevation       — 俯仰角 (度)
#   power           — 检测功率

# frames: structured array
#   frame_id        — 帧序号
#   timestamp       — 时间戳
#   num_points      — 该帧点数
#   offset          — 在 points 数组中的起始偏移
```

---

## 🔄 CFAR 参数调整

检测点数量可通过 CFAR 参数调节：

**4T4R** (`point_cloud_gen.py` → `RadarConfig`):
```python
cfar_guard = 8        # 保护单元
cfar_search = 8       # 搜索单元
cfar_threshold = 8    # 阈值倍数 (越小检测越多)
```

**2T4R** (`point_cloud_gen_2t4r.py` → `RadarConfig2T4R`):
```python
cfar_guard = 2        # 保护单元
cfar_search = 8       # 搜索单元
cfar_threshold = 2    # 阈值倍数
skip_bins = 3         # 跳过近距离 bin
mean_en = 1           # 均值减法: 0=不减, 1=当前帧, 2=前帧
```

---

## 📂 重要文件说明

### 4T4R 相关

| 文件 | 说明 |
|------|------|
| `point_cloud_gen.py` | 主处理管道 (FFT1D~4D + CFAR + DPK) |
| `visualize_pointcloud_smpl.py` | 点云 + SMPL 骨架 3D 可视化 |
| `1-SV4.0.1/` | 硬件驱动 (USB 3.0, FX3, FPGA) |
| `3-matlab/` | MATLAB 校准和信号处理 |
| `5-RADAR_V10/` | 低层 C 算法 (FFT, CFAR, Angle) |

### 2T4R 相关

| 文件 | 说明 |
|------|------|
| `Radar_2T4R/point_cloud_gen_2t4r.py` | 点云生成 (Range/Doppler/Angle FFT + CFAR) |
| `Radar_2T4R/visualize_pointcloud_2t4r.py` | 3D + 2D 交互式点云可视化 |
| `Radar_2T4R/rdmap_2t4r.py` | Range-Doppler Map 生成与可视化 |

---

## 🎯 应用场景

- 人体动作识别 (HAR)
- 手势识别
- 生命体征检测
- 室内定位与追踪
- 自动驾驶目标检测
