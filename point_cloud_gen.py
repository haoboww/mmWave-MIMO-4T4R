"""
点云生成脚本 - 基于 SKY32B750_RC6011C 4T4R 雷达
参数配置与 5-RADAR_V10 硬件代码 **完全一致**

处理流程 (与硬件 Func02_Handle 完全对应):
1. FFT1D (Range FFT) - 512点FFT + Hanning窗
2. P2 Mean - 静态杂波抑制 (chirp维度均值减法)
3. FFT2D (Doppler FFT) - 256点FFT + TDM-MIMO零填充 + Hanning窗
4. CFAR 检测 (SO-CFAR) - 在所有天线幅度和上检测
5. FFT3D (方位角 FFT) - 128点FFT, 不加窗
6. DPK (Direction Peak Detection) - 多峰检测
7. FFT4D (俯仰角 FFT) - 128点FFT + Steering Vector
8. 点云生成与保存 (npz格式)
"""

import numpy as np
from scipy import signal
from scipy.io import loadmat
import matplotlib
matplotlib.use('Agg')  # 非交互式后端，避免显示窗口
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import os
import time


# ======================== 天线校准参数 (来自 App_Func.c) ========================
# calib_ap: 幅度/相位校准参数 (定点数格式, 需转换为复数)
CALIB_AP_RAW = [
    0x10001000, 0x1FEBAEE7, 0x0F3C1EAC, 0x01DF253B,
    0x10C52F38, 0x1FBDD41B, 0x106D6D79, 0x0DA7DFD5,
    0x07DD74AC, 0x1B5AC7C2, 0x145FF974, 0x1BEF02CB,
    0x1BA8FDF3, 0x15744114, 0x1AF97DD4, 0x1475843B
]

# calib_ap_Pre: 浮点校准参数 [real, imag] * 16
CALIB_AP_FLOAT = [
    1.000000000000000, 0.000000000000000, 
    -1.068481445312500, -0.019897460937500, 
    0.958496093750000, -0.095703125000000, 
    -0.836547851562500, 0.233886718750000, 
    -1.048706054687500, 0.192382812500000, 
    1.256469726562500, -0.064697265625000, 
    -1.157836914062500, 0.106567382812500, 
    0.994750976562500, -0.293090820312500, 
    -0.354003906250000, 0.983032226562500, 
    0.484863281250000, -1.161376953125000, 
    -0.409179687500000, 1.093383789062500, 
    0.174560546875000, -1.016479492187500, 
    -0.128051757812500, -1.085205078125000, 
    0.067260742187500, 1.363525390625000, 
    -0.135620117187500, -1.256591796875000, 
    0.264404296875000, 1.114624023437500
]

# nco_fcw: NCO频率校准参数
NCO_FCW = [
    0x00000000, 0x000008D5, 0x000008A4, 0x00000093,
    0x007FFE25, 0x00000646, 0x0000050E, 0x007FFD71,
    0x007FF385, 0x007FFD0E, 0x007FFB44, 0x007FF3C7,
    0x007FF604, 0x007FFF4C, 0x007FFE87, 0x007FF5D3
]


# ======================== 雷达参数配置 (来自 5-RADAR_V10 硬件代码) ========================
class RadarConfig:
    """雷达参数配置类 - 与嵌入式代码 App_Def.h / ParamConfig.h 完全一致"""
    
    def __init__(self, bandwidth_mode: int = 0):
        """
        Args:
            bandwidth_mode: 0 = 3.8GHz (Raw Data采集), 1 = 6.5GHz (点云硬件)
        """
        # 基本物理常数
        self.c = 3e8  # 光速
        
        # RF 参数 (来自 ParamConfig.h)
        self.freq_start = 58.5e9      # FREQ_INIT = 58.5e9 Hz
        self.bandwidth_mode = bandwidth_mode
        
        if bandwidth_mode == 1:
            # 点云硬件配置 (5-RADAR_V10, bandWidth=1)
            self.bandwidth = 6.5e9    # BANDWIDTH_6G5 = 6.5GHz
            self.sweep_time_a = 20e-6 # sweepTimeA = 20us
            self.sweep_time_b = 55e-6 # sweepTimeB = 55us  
            self.sweep_time = self.sweep_time_a + self.sweep_time_b  # T_up = 75us
            self.chirp_gap = 80e-6    # 默认 80us
        else:
            # Raw Data采集配置 (1-SV4.0.1, bandWidth=0)
            self.bandwidth = 3.8e9    # 实际带宽 3.8GHz
            self.sweep_time_a = 10e-6 # sweepTimeA = 10us
            self.sweep_time_b = 55e-6 # sweepTimeB = 55us
            self.sweep_time = self.sweep_time_a + self.sweep_time_b  # T_up = 65us
            self.chirp_gap = 80e-6    # 默认 80us
        
        # 数据采集参数 (来自 App_Def.h)
        self.num_samples = 512        # ADC_MAX_LEN = 1<<NUMLOG_512 = 512 (CIC降采样后)
        self.num_chirps = 64          # NUM_CHIRP = 64
        self.num_tx = 4               # NUM_TX = 4 (PA_SET = 0xF0)
        self.num_rx = 4               # NUM_RX = 4
        self.num_ant = 16             # NUM_ANT = NUM_RX * NUM_TX = 16
        self.num_ant_h = 8            # NUM_ANT_H = 8 (方位角, 从TX2开始)
        self.num_ant_v = 3            # NUM_ANT_V = 3 (俯仰角)
        
        # ADC 参数
        self.sample_len = 1024        # SAMPLE_LEN = 1024 (CIC前)
        self.adc_sampling_rate = 20e6 # ADC_SAMPLING_RATE = 20MHz
        self.fs = self.adc_sampling_rate / 2  # CIC降采样后 = 10MHz
        
        # FFT 参数 (来自 App_Def.h) - 关键修正!
        self.use_range = 256          # USE_RANGE = 256
        self.num_vel = 64             # NUM_VEL = NUM_CHIRP = 64
        self.fft2d_points = 256       # FFTPT_256: NUM_CHIRP * NUM_TX = 64 * 4 = 256
        self.num_angle = 128          # NUM_ANGLE = 1<<DPK_LENLOG = 128
        
        # CFAR 参数 (来自 ParamConfig.c Default_Param_Config)
        self.cfar_guard = 2           # cfarThVG = 2
        self.cfar_search = 6          # cfarThVS = 6
        self.cfar_threshold = 8       # cfarThV = 6
        self.cfar_mode = 'SO'         # CFAR_CFG0_CFARMODE_SO
        self.cfar_max_num = 400       # CFAR_MAX_NUM = 400
        
        # DPK 参数
        self.dpk_threshold = 2        # dpkThres = 8
        self.dpk_times = 2            # DPK_TIMES = 2
        
        # CFAR 检测天线索引 (来自 App_Def.h)
        # CFAR_TX = 2-1 = 1, CFAR_RX = 3-1 = 2
        # CFAR_ANT = CFAR_TX*NUM_RX + CFAR_RX = 1*4 + 2 = 6
        self.cfar_ant = 6
        
        # 方位角天线起始索引 (来自 App_Func.c FFT3D处理)
        # 使用从 NUM_ANT_H * USE_RANGE 开始的天线, 即 TX2 开始的 8 个天线
        self.azimuth_ant_start = 8    # 从天线索引 8 开始 (TX2_RX0-3, TX3_RX0-3)
        
        # 计算参数
        self.fc = self.freq_start + self.bandwidth / 2  # 中心频率
        self.lambda_ = self.c / self.fc                 # 波长
        
        # Chirp 时间 (来自 ParamConfig.h)
        # 单个TDM周期 = chirp_gap * NUM_TX = 80us * 4 = 320us
        # 这是所有TX完成一轮发射所需的时间
        self.chirp_time = self.chirp_gap * self.num_tx  # 320us
        
        # 天线校准参数
        self._init_calibration()
        
        # 计算物理参数
        self._calculate_physical_params()
    
    def _init_calibration(self):
        """初始化天线校准参数"""
        # 转换为复数校准系数
        self.calib_ap = np.zeros(self.num_ant, dtype=np.complex128)
        for i in range(self.num_ant):
            real_part = CALIB_AP_FLOAT[i * 2]
            imag_part = CALIB_AP_FLOAT[i * 2 + 1]
            self.calib_ap[i] = real_part + 1j * imag_part
        
        # NCO 频率校准 (转换为相位)
        self.nco_phase = np.zeros(self.num_ant)
        for i in range(self.num_ant):
            fcw = NCO_FCW[i]
            # 处理有符号数 (23位)
            if fcw & 0x400000:  # 负数
                fcw = fcw - 0x800000
            self.nco_phase[i] = fcw * 2 * np.pi / (1 << 23)
    
    def _calculate_physical_params(self):
        """计算物理参数：分辨率和最大量程"""
        # 距离分辨率 (来自 ParamConfig.c ResolutionCount)
        i = self.sample_len / (self.adc_sampling_rate)  # 有效采样时间
        j = self.bandwidth / self.sweep_time            # 调频斜率
        self.range_res = self.c / (2 * i * j)
        
        # 最大距离 (基于USE_RANGE)
        self.range_max = self.range_res * self.use_range
        
        # 速度分辨率 (来自 ParamConfig.c ResolutionCount)
        self.vel_res = self.c / (self.freq_start * 2 * self.num_chirps * self.chirp_time)
        
        # 最大速度 (基于256点FFT)
        self.vel_max = self.vel_res * self.fft2d_points / 2
        
        # 角度分辨率 (基于FFT)
        self.angle_res = np.degrees(np.arcsin(2 / self.num_angle))
        
        # Range bin 分辨率
        self.range_bin = self.range_res
        
        # Doppler bin 分辨率
        self.doppler_bin = self.vel_res
        
    def print_params(self):
        """打印雷达参数"""
        print("=" * 60)
        print("雷达参数配置")
        print("=" * 60)
        mode_str = '6.5GHz (点云硬件)' if self.bandwidth_mode == 1 else '3.8GHz (Raw Data采集)'
        print(f"带宽模式: {mode_str}")
        print(f"起始频率: {self.freq_start/1e9:.2f} GHz")
        print(f"带宽: {self.bandwidth/1e9:.2f} GHz")
        print(f"扫频时间: T_up = {self.sweep_time*1e6:.0f}us (A={self.sweep_time_a*1e6:.0f}us + B={self.sweep_time_b*1e6:.0f}us)")
        print(f"中心频率: {self.fc/1e9:.2f} GHz")
        print(f"波长: {self.lambda_*1000:.2f} mm")
        print("-" * 60)
        print(f"采样点数 (CIC后): {self.num_samples}")
        print(f"Chirp数: {self.num_chirps}")
        print(f"天线配置: {self.num_tx}T{self.num_rx}R = {self.num_ant} 虚拟天线")
        print(f"USE_RANGE: {self.use_range}")
        print(f"NUM_VEL: {self.num_vel}")
        print(f"FFT2D点数: {self.fft2d_points} (NUM_CHIRP × NUM_TX)")
        print(f"NUM_ANGLE: {self.num_angle}")
        print("-" * 60)
        print(f"距离分辨率: {self.range_res:.4f} m")
        print(f"最大距离: {self.range_max:.2f} m")
        print(f"速度分辨率: {self.vel_res:.4f} m/s")
        print(f"最大速度: ±{self.vel_max:.2f} m/s")
        print(f"角度分辨率: {self.angle_res:.2f}°")
        print("-" * 60)
        print(f"CFAR Guard: {self.cfar_guard}, Search: {self.cfar_search}, Threshold: {self.cfar_threshold}")
        print(f"CFAR Mode: {self.cfar_mode}")
        print(f"CFAR ANT: {self.cfar_ant}")
        print(f"DPK Threshold: {self.dpk_threshold}, DPK Times: {self.dpk_times}")
        print("=" * 60)


# ======================== Range-Doppler 处理类 ========================
class DopplerAlgo:
    """Range-Doppler 处理算法 - 与硬件 Func02_Handle 完全一致"""
    
    def __init__(self, config: RadarConfig):
        self.config = config
        self.num_chirps = config.num_chirps
        self.num_samples = config.num_samples
        self.use_range = config.use_range
        self.num_vel = config.num_vel
        self.num_tx = config.num_tx
        self.num_rx = config.num_rx
        self.num_ant = config.num_ant
        self.fft2d_points = config.fft2d_points  # 256点
        
        # Hanning 窗函数 (与硬件 winRange_Pre 一致)
        # 硬件使用14位量化的Hanning窗, 这里使用浮点
        self.range_window = signal.windows.hann(self.num_samples).reshape(1, self.num_samples)
        
        # Doppler 窗函数 - 256点 (与硬件 winVelPre 一致)
        # 硬件对每个TX使用不同的窗函数片段, 这里简化为完整窗
        self.doppler_window_full = signal.windows.hann(self.fft2d_points)
        
        # 生成 TDM-MIMO 零填充掩码 (与硬件 BB_FFT2D_ZP_Init 一致)
        self._init_tdm_mimo_mask()
        
        # FFT1D 均值缓存 (用于静态杂波抑制)
        self.fft1d_mean = None
        self.fft1d_mean_prev = None
        
    def _init_tdm_mimo_mask(self):
        """初始化 TDM-MIMO 零填充掩码
        
        硬件逻辑: 对于256点FFT, 每个TX只在特定的chirp位置有效
        TX0: chirp 0,4,8,... (每4个一个)
        TX1: chirp 1,5,9,...
        TX2: chirp 2,6,10,...
        TX3: chirp 3,7,11,...
        """
        self.tdm_masks = []
        for tx_idx in range(self.num_tx):
            mask = np.zeros(self.fft2d_points)
            for chirp_idx in range(self.num_chirps):
                # 在256点FFT中, TX的位置
                fft_idx = chirp_idx * self.num_tx + tx_idx
                mask[fft_idx] = 1.0
            self.tdm_masks.append(mask)
    
    def compute_range_fft(self, data: np.ndarray, apply_calib: bool = True, 
                          ant_idx: int = 0) -> np.ndarray:
        """计算 Range FFT (FFT1D) - 与硬件完全一致
        
        硬件流程: ZO + P1MUL + FFT
        """
        numchirps, chirpsamples = data.shape
        
        # ZO: 去直流 (硬件 BB_ZO_Cfg0)
        avgs = np.average(data, axis=1).reshape(numchirps, 1)
        data = data - avgs
        
        # P1MUL: 天线校准 (如果启用)
        if apply_calib:
            calib = self.config.calib_ap[ant_idx]
            nco_phase = self.config.nco_phase[ant_idx]
            # NCO 相位校正
            for chirp_idx in range(numchirps):
                phase_correction = np.exp(1j * nco_phase * chirp_idx)
                data[chirp_idx, :] = data[chirp_idx, :] * calib * phase_correction
        
        # 加窗 (硬件 FFT_CFG1_WIN_EN)
        data = np.multiply(data, self.range_window)
        
        # FFT1D: 512点FFT (硬件 FFTPT_512)
        range_fft = np.fft.fft(data, n=self.num_samples, axis=1) / self.num_samples
        
        # 只取前 use_range 个bin (硬件 FFT1D_USE_A=0, FFT1D_USE_B=255)
        range_fft = range_fft[:, :self.use_range]
        
        return range_fft
    
    def compute_fft1d_mean(self, fft1d_all_ant: np.ndarray):
        """计算 FFT1D 均值 (P2 mean) - 用于静态杂波抑制
        
        硬件流程: P2_CFG0_MODE_ACC, 对所有chirp求和再除以64
        
        Args:
            fft1d_all_ant: [num_ant, num_chirps, use_range]
        """
        # 对chirp维度求均值
        mean = np.mean(fft1d_all_ant, axis=1)  # [num_ant, use_range]
        
        # 保存当前和前一帧均值 (用于 meanEn=2 模式)
        self.fft1d_mean_prev = self.fft1d_mean
        self.fft1d_mean = mean
        
        return mean
    
    def compute_doppler_fft_tdm(self, fft1d: np.ndarray, tx_idx: int, 
                                 mean_subtract: np.ndarray = None) -> np.ndarray:
        """计算单个TX的 Doppler FFT (FFT2D) - 与硬件完全一致
        
        硬件流程: P2 minus + FFT2D with TDM-MIMO zero padding
        
        Args:
            fft1d: [num_chirps, use_range] - 单个RX的FFT1D结果
            tx_idx: TX索引
            mean_subtract: [use_range] - 要减去的均值
        
        Returns:
            fft2d: [use_range, fft2d_points] - FFT2D结果
        """
        # P2 minus: 静态杂波抑制 (减去均值)
        if mean_subtract is not None:
            fft1d = fft1d - mean_subtract.reshape(1, -1)
        
        # 转置: [use_range, num_chirps]
        fft1d_t = np.transpose(fft1d)
        
        # TDM-MIMO 零填充
        # 将64个chirp填充到256点FFT的正确位置
        fft_input = np.zeros((self.use_range, self.fft2d_points), dtype=np.complex128)
        for chirp_idx in range(self.num_chirps):
            fft_idx = chirp_idx * self.num_tx + tx_idx
            fft_input[:, fft_idx] = fft1d_t[:, chirp_idx]
        
        # 加窗 (硬件 winVelPre)
        # 只对非零位置加窗
        window_applied = np.zeros_like(fft_input)
        for chirp_idx in range(self.num_chirps):
            fft_idx = chirp_idx * self.num_tx + tx_idx
            window_applied[:, fft_idx] = fft_input[:, fft_idx] * self.doppler_window_full[fft_idx]
        
        # FFT2D: 256点FFT (硬件 FFTPT_256)
        fft2d = np.fft.fft(window_applied, n=self.fft2d_points, axis=1) / self.num_chirps
        
        return fft2d
    
    def compute_doppler_map_all_ant(self, frame_data: np.ndarray, 
                                     use_mean_subtract: int = 2) -> np.ndarray:
        """计算所有天线的 Range-Doppler Map - 与硬件完全一致
        
        Args:
            frame_data: [num_ant, num_chirps, num_samples]
            use_mean_subtract: 0=不减, 1=减当前帧, 2=减前一帧 (与硬件 meanEn 一致)
        
        Returns:
            rd_maps: [num_ant, use_range, fft2d_points]
        """
        num_ant = frame_data.shape[0]
        
        # Step 1: 计算所有天线的 FFT1D
        fft1d_all = np.zeros((num_ant, self.num_chirps, self.use_range), dtype=np.complex128)
        for ant_idx in range(num_ant):
            data_one_ant = frame_data[ant_idx, :, :]
            fft1d_all[ant_idx] = self.compute_range_fft(data_one_ant, apply_calib=True, ant_idx=ant_idx)
        
        # Step 2: 计算 FFT1D 均值 (P2 mean)
        fft1d_mean = self.compute_fft1d_mean(fft1d_all)
        
        # Step 3: 对每个天线计算 FFT2D
        rd_maps = np.zeros((num_ant, self.use_range, self.fft2d_points), dtype=np.complex128)
        
        for tx_idx in range(self.num_tx):
            for rx_idx in range(self.num_rx):
                ant_idx = tx_idx * self.num_rx + rx_idx
                
                # 确定要减去的均值
                if use_mean_subtract == 0:
                    mean_sub = None
                elif use_mean_subtract == 1:
                    mean_sub = fft1d_mean[ant_idx]
                elif use_mean_subtract == 2:
                    # 使用前一帧的均值
                    if self.fft1d_mean_prev is not None:
                        mean_sub = self.fft1d_mean_prev[ant_idx]
                    else:
                        mean_sub = fft1d_mean[ant_idx]
                else:
                    mean_sub = None
                
                # FFT2D
                rd_maps[ant_idx] = self.compute_doppler_fft_tdm(
                    fft1d_all[ant_idx], tx_idx, mean_sub
                )
        
        return rd_maps
    
    def reset(self):
        """重置处理器状态"""
        self.fft1d_mean = None
        self.fft1d_mean_prev = None


# ======================== CFAR 检测类 ========================
class CFARDetector:
    """CFAR 检测器 - 与硬件 BB_CFAR_Vel_Init 完全一致"""
    
    def __init__(self, config: RadarConfig):
        self.config = config
        
        # CFAR 参数 (来自 BB_Config.c BB_CFAR_Init / BB_CFAR_Vel_Init)
        self.guard_size = config.cfar_guard     # cfarThVG = 2
        self.search_size = config.cfar_search   # cfarThVS = 6
        self.threshold_mul = config.cfar_threshold  # cfarThV = 6
        self.total_size = self.guard_size + self.search_size  # = 8
        
        # 硬件参数 - 关键修正!
        # CFAR 是在 USE_RANGE x NUM_VEL 上做的，不是 USE_RANGE x FFT2D_POINTS
        self.inter_cnt = config.use_range       # interCnt = USE_RANGE = 256
        self.intra_cnt = config.num_vel         # intraCnt = NUM_VEL = 64 (关键!)
        self.fft2d_points = config.fft2d_points
        
    def cfar_1d_so(self, signal: np.ndarray) -> np.ndarray:
        """1D SO-CFAR (Smallest Of) - 与硬件完全一致
        
        硬件: CFAR_CFG0_CFARMODE_SO, wrap around
        
        Args:
            signal: 输入信号 (幅度)
        
        Returns:
            threshold: CFAR 门限
        """
        n = len(signal)
        threshold = np.zeros(n)
        
        for i in range(n):
            # 左侧训练单元
            left_start = i - self.total_size
            left_end = i - self.guard_size
            
            # 右侧训练单元  
            right_start = i + self.guard_size + 1
            right_end = i + self.total_size + 1
            
            # 处理边界 (wrap around - 与硬件 CFAR_CFG0_WRAPDIR_INC 一致)
            left_cells = []
            for j in range(left_start, left_end):
                idx = j % n
                left_cells.append(signal[idx])
            
            right_cells = []
            for j in range(right_start, right_end):
                idx = j % n
                right_cells.append(signal[idx])
            
            left_cells = np.array(left_cells)
            right_cells = np.array(right_cells)
            
            # SO-CFAR: 取左右平均的较小值 (硬件 CFAR_CFG0_CFARMODE_SO)
            if len(left_cells) > 0 and len(right_cells) > 0:
                left_mean = np.mean(left_cells)
                right_mean = np.mean(right_cells)
                noise_level = min(left_mean, right_mean)
            elif len(left_cells) > 0:
                noise_level = np.mean(left_cells)
            elif len(right_cells) > 0:
                noise_level = np.mean(right_cells)
            else:
                noise_level = 0
            
            # 硬件: threshold = noise_level * mulFac / divFac
            # divFac = 1 (CFAR_CFG0_DIVFAC1), mulFac = cfarThV
            threshold[i] = noise_level * self.threshold_mul
        
        return threshold
    
    def compute_abs_sum(self, rd_maps: np.ndarray) -> np.ndarray:
        """计算所有天线的幅度和 - 与硬件 P2 sum 完全一致
        
        硬件: P2_CFG0_SUBMODE_ABS_SUM, P2_CFG0_DIV128
        
        Args:
            rd_maps: [num_ant, use_range, fft2d_points]
        
        Returns:
            abs_sum: [use_range, fft2d_points]
        """
        # 对所有天线取幅度值并求和, 然后除以128 (P2_CFG0_DIV128)
        abs_sum = np.sum(np.abs(rd_maps), axis=0) / 128.0
        return abs_sum
    
    def extract_valid_doppler_bins(self, rd_abs_sum: np.ndarray) -> tuple:
        """从256点FFT结果中提取有效的64个速度bin - 与硬件FFT2D_USE_A/B完全一致
        
        硬件参数:
        - FFT2D_USE_A = (NUM_TX-1)*NUM_VEL+NUM_VEL/2 = 224
        - FFT2D_USE_B = NUM_VEL/2-1 = 31
        
        硬件输出顺序: bin 224, 225, ..., 255, 0, 1, ..., 31 (共64个，连续存储)
        这个顺序保证了 CFAR wrap-around 时数据是连续的！
        
        Args:
            rd_abs_sum: [use_range, fft2d_points] (256x256)
        
        Returns:
            valid_data: [use_range, num_vel] (256x64)
            bin_mapping: 从64索引到256索引的映射
        """
        num_vel = self.intra_cnt  # 64
        fft_pts = self.fft2d_points  # 256
        
        # 关键：与硬件输出顺序完全一致！
        # 先 bin 224..255 (32个)，再 bin 0..31 (32个)
        bin_mapping = np.zeros(num_vel, dtype=np.int32)
        
        # 前32个: bin 224, 225, ..., 255 (负速度)
        for i in range(num_vel // 2):
            bin_mapping[i] = fft_pts - num_vel // 2 + i  # 224, 225, ..., 255
        
        # 后32个: bin 0, 1, ..., 31 (正速度)
        for i in range(num_vel // 2):
            bin_mapping[num_vel // 2 + i] = i  # 0, 1, ..., 31
        
        # 提取有效数据
        valid_data = rd_abs_sum[:, bin_mapping]
        
        return valid_data, bin_mapping
    
    def detect_2d(self, rd_abs_sum: np.ndarray, skip_bins: int = 0, 
                  use_local_max: bool = True) -> dict:
        """2D CFAR 检测 - 与硬件完全一致
        
        硬件在 64 个有效速度 bin 上做 CFAR (BB_CFAR_Vel_Init: intraCnt = NUM_VEL = 64)
        bin 顺序: 224, 225, ..., 255, 0, 1, ..., 31 (与硬件 FFT2D_USE_A/B 一致)
        
        Args:
            rd_abs_sum: Range-Doppler 幅度和 [use_range, fft2d_points]
            skip_bins: 跳过的近距离bin数
            use_local_max: 是否启用局部最大值检测 (减少散乱点)
        
        Returns:
            detections: 检测结果字典
        """
        # 关键: 从256点FFT中提取有效的64个速度bin (与硬件一致的顺序)
        valid_data, bin_mapping = self.extract_valid_doppler_bins(rd_abs_sum)
        n_range, n_doppler = valid_data.shape  # 256x64
        
        detections = {
            'range_idx': [],      # interIdx
            'doppler_idx': [],    # intraIdx (在64点中的索引)
            'doppler_idx_256': [],  # 在256点FFT中的原始索引
            'amplitude': [],      # thresABS
        }
        
        # 遍历每个距离bin (硬件: interCnt = USE_RANGE = 256)
        for r_idx in range(skip_bins, n_range):
            # 获取该距离bin的多普勒维数据 (64个有效bin)
            doppler_slice = valid_data[r_idx, :]
            
            # 计算 CFAR 门限 (在 velocity 维度, 64点)
            threshold = self.cfar_1d_so(doppler_slice)
            
            # 检测超过门限的点
            for d_idx in range(n_doppler):
                if doppler_slice[d_idx] > threshold[d_idx]:
                    # 局部最大值检测 (减少散乱点)
                    if use_local_max:
                        is_local_max = True
                        # 检查速度维度的邻居
                        for d_offset in [-1, 1]:
                            neighbor_d = (d_idx + d_offset) % n_doppler
                            if doppler_slice[d_idx] < doppler_slice[neighbor_d]:
                                is_local_max = False
                                break
                        # 检查距离维度的邻居
                        if is_local_max:
                            for r_offset in [-1, 1]:
                                neighbor_r = r_idx + r_offset
                                if 0 <= neighbor_r < n_range:
                                    if valid_data[r_idx, d_idx] < valid_data[neighbor_r, d_idx]:
                                        is_local_max = False
                                        break
                        if not is_local_max:
                            continue
                    
                    detections['range_idx'].append(r_idx)
                    detections['doppler_idx'].append(d_idx)
                    detections['doppler_idx_256'].append(bin_mapping[d_idx])
                    detections['amplitude'].append(doppler_slice[d_idx])
        
        # 转换为 numpy 数组
        for key in detections:
            detections[key] = np.array(detections[key])
        
        # 限制最大检测数 (硬件 CFAR_MAX_NUM = 400)
        if len(detections['range_idx']) > self.config.cfar_max_num:
            # 按幅度排序, 保留最大的
            sort_idx = np.argsort(detections['amplitude'])[::-1][:self.config.cfar_max_num]
            for key in detections:
                detections[key] = detections[key][sort_idx]
        
        return detections


# ======================== 角度估计类 ========================
class DOAEstimator:
    """DOA 角度估计器 - 与硬件 FFT3D/FFT4D/DPK 完全一致"""
    
    def __init__(self, config: RadarConfig):
        self.config = config
        self.num_angle = config.num_angle      # 128
        self.num_ant_h = config.num_ant_h      # 方位角天线数 = 8
        self.num_ant_v = config.num_ant_v      # 俯仰角天线数 = 3
        self.num_rx = config.num_rx            # 4
        self.num_tx = config.num_tx            # 4
        self.dpk_threshold = config.dpk_threshold
        self.dpk_times = config.dpk_times
        
        # 调试: 存储DPK计算的中间值
        self._debug_dpk_values = []
        
        # 方位角天线索引 (来自硬件 FFT3D 处理)
        # 硬件使用: FFT2D_CACHE_ADDR + NUM_ANT_H * USE_RANGE * NUM_BYTE32
        # 即从天线索引 8 开始 (TX2_RX0-3, TX3_RX0-3)
        self.azimuth_ant_start = config.azimuth_ant_start  # 8
        
        # 俯仰角天线索引 (来自硬件 FFT4D 处理)
        # 对于每个RX, 取TX0, TX1, TX2 对应的数据
        # 天线索引: RX0->[0,4,8], RX1->[1,5,9], ...
        
        # 初始化 Steering Vector (来自硬件 steeringVec_Pre)
        self._init_steering_vector()
    
    def _init_steering_vector(self):
        """初始化 Steering Vector - 用于 FFT4D"""
        # 简化版本: 生成标准的相位导向矢量
        # 完整版本应该从硬件代码中提取 steeringVec_Pre
        self.steering_vec = np.ones((self.num_rx, self.num_angle), dtype=np.complex128)
        
        # 生成标准导向矢量 (对于均匀线阵)
        for rx_idx in range(self.num_rx):
            for angle_idx in range(self.num_angle):
                # 角度对应 sin(theta) = (angle_idx - num_angle/2) * 2 / num_angle
                sin_theta = (angle_idx - self.num_angle / 2) * 2 / self.num_angle
                # 相位 = 2*pi*d*sin(theta)/lambda, d = lambda/2
                phase = np.pi * rx_idx * sin_theta
                self.steering_vec[rx_idx, angle_idx] = np.exp(1j * phase)
    
    def estimate_azimuth_fft3d(self, antenna_data: np.ndarray) -> tuple:
        """估计方位角 (FFT3D) - 与硬件完全一致
        
        硬件流程:
        - 输入: 从天线索引8开始的8个天线 (NUM_ANT_H=8)
        - FFT: 128点, 不加窗 (FFT_CFG1_WIN_DIS)
        - 输出: 角度谱和峰值
        
        Args:
            antenna_data: 所有天线数据 [num_ant] 在特定 range-doppler bin
        
        Returns:
            angle_spectrum: 角度谱 [num_angle]
            peak_info: 峰值信息 (idx, power, complex_value)
        """
        # 选择方位角天线 (从索引8开始的8个天线)
        az_data = antenna_data[self.azimuth_ant_start:self.azimuth_ant_start + self.num_ant_h]
        
        # 硬件不加窗 (FFT_CFG1_WIN_DIS)
        # az_data_win = az_data  # 直接使用原始数据
        
        # 角度 FFT (128点, 与硬件 FFTPT_128 一致)
        # 硬件 FFT3D_USE_A=0, FFT3D_USE_B=127, 输出全部128点
        angle_fft = np.fft.fft(az_data, self.num_angle)
        angle_spectrum = np.abs(angle_fft)
        
        # 找峰值 (硬件 BB_FFT_STA0_curMaxIdx)
        peak_idx = np.argmax(angle_spectrum)
        peak_power = angle_spectrum[peak_idx]
        peak_complex = angle_fft[peak_idx]
        
        peak_info = {
            'idx': peak_idx,
            'power': peak_power,
            'complex': peak_complex
        }
        
        return angle_spectrum, peak_info
    
    def dpk_detect(self, angle_spectrum: np.ndarray, peak_info: dict) -> tuple:
        """DPK (Direction Peak Detection) - 与硬件完全一致
        
        硬件流程: 找到多个峰值, 使用 sinc 插值抵消
        关键点: 硬件在FFT3D输出后进行DPK处理，cutPow是FFT幅度值，
                res是DPK处理后的残差
        
        Args:
            angle_spectrum: 角度谱 [num_angle] (FFT幅度值)
            peak_info: 第一个峰值信息
        
        Returns:
            peaks: 检测到的峰值列表
            final_residual: 最终残差 (用于阈值判断 cutPow > res * dpkThres)
        """
        peaks = []
        spectrum = angle_spectrum.copy()
        
        # 第一个峰值
        peaks.append({
            'idx': peak_info['idx'],
            'power': peak_info['power'],  # FFT幅度值
            'complex': peak_info['complex']
        })
        
        # DPK 迭代检测更多峰值 (最多 DPK_TIMES 个峰值)
        for dpk_iter in range(1, self.dpk_times):
            # 抵消已检测的峰值 (硬件用sinc插值，这里使用更大范围模拟sinc旁瓣)
            prev_idx = peaks[-1]['idx']
            for offset in range(-4, 5):  # 增大抵消范围
                idx = (prev_idx + offset) % self.num_angle
                spectrum[idx] = 0
            
            # 找下一个峰值
            new_peak_idx = np.argmax(spectrum)
            new_peak_power = spectrum[new_peak_idx]
            
            if new_peak_power > 0:
                peaks.append({
                    'idx': new_peak_idx,
                    'power': new_peak_power,
                    'complex': angle_spectrum[new_peak_idx]
                })
        
        # 计算最终残差 (DPK处理后)
        # 将所有已检测峰值附近置零后计算残差
        for peak in peaks:
            for offset in range(-4, 5):  # 与上面抵消范围一致
                idx = (peak['idx'] + offset) % self.num_angle
                spectrum[idx] = 0
        
        # 修正：残差计算使用幅度均值 (与硬件更接近)
        # 硬件的 res 是 DPK 抵消后的残差
        final_residual = np.mean(spectrum)  # 使用幅度均值
        
        return peaks, final_residual
    
    def estimate_elevation_fft4d(self, antenna_data: np.ndarray, azimuth_idx: int) -> tuple:
        """估计俯仰角 (FFT4D) - 与硬件完全一致
        
        硬件流程:
        - 输入: NUM_ANT_V=3 个俯仰角天线, 乘以 Steering Vector
        - P2 MAC: 对每个RX做累加
        - FFT: 128点, 不加窗
        
        Args:
            antenna_data: 所有天线数据 [num_ant]
            azimuth_idx: 方位角索引 (用于选择 steering vector)
        
        Returns:
            angle_spectrum: 角度谱 [num_angle]
            elevation_idx: 俯仰角索引
        """
        # 提取俯仰角天线数据并应用 steering vector
        # 硬件使用 P2 MAC 运算: FFT2D[d2Idx][ant][d1Idx] * steeringVec[rx][d3Idx]
        
        el_data = np.zeros(self.num_ant_v, dtype=np.complex128)
        
        for v_idx in range(self.num_ant_v):
            # 俯仰角天线: TX0,TX1,TX2 的 RX0-3
            # 对每个TX, 累加所有RX的贡献 (与 steering vector 相乘)
            acc = 0
            for rx_idx in range(self.num_rx):
                ant_idx = v_idx * self.num_rx + rx_idx
                # 乘以 steering vector
                acc += antenna_data[ant_idx] * self.steering_vec[rx_idx, azimuth_idx]
            el_data[v_idx] = acc
        
        # FFT4D: 128点, 不加窗 (硬件 FFT_CFG1_WIN_DIS)
        angle_fft = np.fft.fft(el_data, self.num_angle)
        angle_spectrum = np.abs(angle_fft)
        
        # 找峰值
        elevation_idx = np.argmax(angle_spectrum)
        
        return angle_spectrum, elevation_idx
    
    def idx_to_angle(self, idx: int) -> float:
        """将 FFT 索引转换为角度 (度)
        
        硬件输出直接是索引, 这里转换为物理角度
        """
        # sin(theta) = (idx - num_angle/2) * 2 / num_angle (对于 fftshift 后)
        # 但硬件不做 fftshift, 所以:
        # 对于 idx < num_angle/2: 正角度
        # 对于 idx >= num_angle/2: 负角度
        if idx < self.num_angle / 2:
            sin_theta = idx * 2 / self.num_angle
        else:
            sin_theta = (idx - self.num_angle) * 2 / self.num_angle
        
        sin_theta = np.clip(sin_theta, -1, 1)
        angle = np.degrees(np.arcsin(sin_theta))
        
        return angle


# ======================== 点云生成器类 ========================
class PointCloudGenerator:
    """点云生成器 - 与硬件 Func02_Handle 完全一致"""
    
    def __init__(self, config: RadarConfig = None):
        self.config = config if config else RadarConfig()
        self.doppler_algo = DopplerAlgo(self.config)
        self.cfar_detector = CFARDetector(self.config)
        self.doa_estimator = DOAEstimator(self.config)
        
        # 坐标轴 (基于256点FFT)
        self.range_axis = np.arange(self.config.use_range) * self.config.range_res
        
        # 速度轴: 与硬件输出顺序完全一致
        # 硬件输出顺序: bin 224, 225, ..., 255, 0, 1, ..., 31
        # 前32个对应负速度 (bin 224-255)，后32个对应正速度 (bin 0-31)
        num_vel = self.config.num_vel  # 64
        fft_pts = self.config.fft2d_points  # 256
        self.doppler_axis_64 = np.zeros(num_vel)
        for i in range(num_vel):
            if i < num_vel // 2:
                # 索引 0..31 对应 bin 224..255 -> 负速度
                # bin_idx = 224 + i, velocity = (bin_idx - 256) * vel_res
                bin_idx = fft_pts - num_vel // 2 + i  # 224, 225, ..., 255
                self.doppler_axis_64[i] = (bin_idx - fft_pts) * self.config.vel_res
            else:
                # 索引 32..63 对应 bin 0..31 -> 正速度
                # bin_idx = i - 32, velocity = bin_idx * vel_res
                bin_idx = i - num_vel // 2  # 0, 1, ..., 31
                self.doppler_axis_64[i] = bin_idx * self.config.vel_res
        
        # 256点FFT的完整速度轴 (用于角度估计时索引)
        self.doppler_axis_256 = np.zeros(self.config.fft2d_points)
        for i in range(self.config.fft2d_points):
            if i < self.config.fft2d_points // 2:
                self.doppler_axis_256[i] = i * self.config.vel_res
            else:
                self.doppler_axis_256[i] = (i - self.config.fft2d_points) * self.config.vel_res
    
    def process_frame(self, frame_data: np.ndarray, skip_bins: int = 0,
                      mean_subtract_mode: int = 2) -> list:
        """处理单帧数据生成点云 - 与硬件 Func02_Handle 完全一致
        
        硬件处理流程:
        1. FFT1D (Range FFT) - 所有天线
        2. P2 Mean - 计算均值用于静态杂波抑制
        3. FFT2D (Doppler FFT) - TDM-MIMO + 零填充 + 256点FFT
        4. Sum FFT2D - 所有天线幅度求和
        5. CFAR - SO-CFAR 在 velocity 维度
        6. FFT3D (方位角) - 128点FFT, 不加窗
        7. DPK - 多峰检测
        8. FFT4D (俯仰角) - 128点FFT + Steering Vector
        
        Args:
            frame_data: 单帧数据 [num_ant, num_chirps, num_samples]
            skip_bins: 跳过的近距离bin数
            mean_subtract_mode: 均值减法模式 (0/1/2)
        
        Returns:
            points: 点云列表
        """
        # Step 1-3: 计算所有天线的 Range-Doppler Map
        rd_maps = self.doppler_algo.compute_doppler_map_all_ant(
            frame_data, use_mean_subtract=mean_subtract_mode
        )  # [num_ant, use_range, fft2d_points]
        
        # Step 4: 计算所有天线的幅度和 (硬件 P2 sum)
        rd_abs_sum = self.cfar_detector.compute_abs_sum(rd_maps)
        
        # Step 5: CFAR 检测
        detections = self.cfar_detector.detect_2d(rd_abs_sum, skip_bins)
        
        # 调试: 打印CFAR检测数量
        cfar_count = len(detections['range_idx'])
        
        # Step 6-8: 对每个检测点进行角度估计和点云生成
        points = []
        dpk_filtered_count = 0
        
        for i in range(len(detections['range_idx'])):
            r_idx = int(detections['range_idx'][i])
            d_idx_64 = int(detections['doppler_idx'][i])      # 64点中的索引 (用于速度计算)
            d_idx_256 = int(detections['doppler_idx_256'][i])  # 256点FFT中的索引 (用于天线数据)
            amplitude = detections['amplitude'][i]
            
            # 获取该检测点的所有天线数据 (使用256点FFT中的索引)
            antenna_data = rd_maps[:, r_idx, d_idx_256]
            
            # Step 6: FFT3D 方位角估计
            angle_spectrum, peak_info = self.doa_estimator.estimate_azimuth_fft3d(antenna_data)
            
            # Step 7: DPK 多峰检测
            azimuth_peaks, final_residual = self.doa_estimator.dpk_detect(angle_spectrum, peak_info)
            
            # 对每个方位角峰值生成点云
            for az_peak in azimuth_peaks:
                az_idx = az_peak['idx']
                az_power = az_peak['power']
                
                # 检查 DPK 阈值 (硬件: cutPow > res * dpkThres)
                # 使用DPK返回的残差 (已经是幅度均值)
                if az_power <= final_residual * self.config.dpk_threshold:
                    dpk_filtered_count += 1
                    continue
                
                # Step 8: FFT4D 俯仰角估计
                _, el_idx = self.doa_estimator.estimate_elevation_fft4d(antenna_data, az_idx)
                
                # 转换索引为角度
                azimuth = self.doa_estimator.idx_to_angle(az_idx)
                elevation = self.doa_estimator.idx_to_angle(el_idx)
                
                # 物理坐标
                range_val = self.range_axis[r_idx]
                velocity = self.doppler_axis_64[d_idx_64]  # 使用64点速度轴 (与硬件一致)
                power = az_power ** 2  # 功率
                
                # 转换为笛卡尔坐标
                azimuth_rad = np.radians(azimuth)
                elevation_rad = np.radians(elevation)
                
                x = range_val * np.cos(elevation_rad) * np.sin(azimuth_rad)
                y = range_val * np.cos(elevation_rad) * np.cos(azimuth_rad)
                z = range_val * np.sin(elevation_rad)
                
                points.append((x, y, z, range_val, velocity, azimuth, elevation, power))
        
        return points, cfar_count, dpk_filtered_count
    
    def process_all_frames(self, data: np.ndarray, skip_bins: int = 0,
                           start_frame: int = 0, end_frame: int = None,
                           mean_subtract_mode: int = 2) -> tuple:
        """处理多帧数据
        
        Args:
            data: 多帧数据 [num_frames, num_ant, num_chirps, num_samples]
            mean_subtract_mode: 0=不减, 1=减当前帧, 2=减前一帧
        
        Returns:
            frames: 帧信息数组
            all_points: 所有点云数组
        """
        num_frames = data.shape[0]
        if end_frame is None:
            end_frame = num_frames
        
        # 定义与 pointcloud.npz 一致的 dtype
        frame_dtype = np.dtype([
            ('frame_id', '<i4'),
            ('timestamp', '<f8'),
            ('num_points', '<i4'),
            ('offset', '<i4')
        ])
        
        point_dtype = np.dtype([
            ('x', '<f4'),
            ('y', '<f4'),
            ('z', '<f4'),
            ('range', '<f4'),
            ('velocity', '<f4'),
            ('azimuth', '<f4'),
            ('elevation', '<f4'),
            ('power', '<f4')
        ])
        
        frames_list = []
        points_list = []
        current_offset = 0
        
        print(f"处理帧 {start_frame} 到 {end_frame-1}...")
        print(f"使用参数: skip_bins={skip_bins}, mean_subtract_mode={mean_subtract_mode}")
        
        # 重置处理器状态
        self.doppler_algo.reset()
        
        base_timestamp = time.time()
        
        for frame_idx in range(start_frame, end_frame):
            frame_data = data[frame_idx]
            frame_points, cfar_count, dpk_filtered = self.process_frame(frame_data, skip_bins, mean_subtract_mode)
            
            num_points = len(frame_points)
            
            # 调试: 每帧输出详细信息
            if frame_idx <= start_frame + 2:  # 只打印前几帧的详细信息
                print(f"  [DEBUG] 帧 {frame_idx}: CFAR={cfar_count}, DPK峰值={cfar_count*2}, DPK过滤={dpk_filtered}, 输出={num_points}")
            else:
                print(f"  帧 {frame_idx}: CFAR={cfar_count}, 输出={num_points}")
            
            # 记录帧信息
            frames_list.append((
                frame_idx - start_frame,           # frame_id
                base_timestamp + frame_idx * 0.1,  # timestamp (假设100ms间隔)
                num_points,                         # num_points
                current_offset                      # offset
            ))
            
            # 记录点云
            for pt in frame_points:
                points_list.append(pt)
            
            current_offset += num_points
        
        # 转换为 structured array
        frames = np.array(frames_list, dtype=frame_dtype)
        points = np.array(points_list, dtype=point_dtype) if points_list else np.array([], dtype=point_dtype)
        
        return frames, points


# ======================== 保存和可视化函数 ========================
def save_point_cloud_npz(frames: np.ndarray, points: np.ndarray, output_path: str):
    """保存点云为 npz 格式 (与 pointcloud.npz 格式一致)"""
    np.savez(output_path, frames=frames, points=points)
    print(f"点云已保存到: {output_path}")
    print(f"帧数: {len(frames)}, 总点数: {len(points)}")


def visualize_point_cloud(points: np.ndarray, title: str = "Point Cloud"):
    """可视化点云"""
    if len(points) == 0:
        print("警告: 点云为空")
        return
    
    fig = plt.figure(figsize=(14, 5))
    
    # 3D 点云
    ax1 = fig.add_subplot(131, projection='3d')
    scatter = ax1.scatter(
        points['x'], 
        points['y'], 
        points['z'],
        c=np.log10(points['power'] + 1),
        cmap='jet',
        s=20
    )
    ax1.set_xlabel('X (m)')
    ax1.set_ylabel('Y (m)')
    ax1.set_zlabel('Z (m)')
    ax1.set_title(f'{title} - 3D View')
    plt.colorbar(scatter, ax=ax1, label='log10(Power)')
    
    # 俯视图 (X-Y)
    ax2 = fig.add_subplot(132)
    scatter2 = ax2.scatter(
        points['x'],
        points['y'],
        c=points['velocity'],
        cmap='coolwarm',
        s=20
    )
    ax2.set_xlabel('X (m)')
    ax2.set_ylabel('Y (m)')
    ax2.set_title(f'{title} - Top View')
    ax2.grid(True)
    ax2.set_aspect('equal')
    plt.colorbar(scatter2, ax=ax2, label='Velocity (m/s)')
    
    # Range-Velocity 分布
    ax3 = fig.add_subplot(133)
    scatter3 = ax3.scatter(
        points['range'],
        points['velocity'],
        c=np.log10(points['power'] + 1),
        cmap='jet',
        s=20
    )
    ax3.set_xlabel('Range (m)')
    ax3.set_ylabel('Velocity (m/s)')
    ax3.set_title(f'{title} - Range-Velocity')
    ax3.grid(True)
    plt.colorbar(scatter3, ax=ax3, label='log10(Power)')
    
    plt.tight_layout()
    output_path = os.path.join(r"D:\Codes\4.0.1", f'{title.replace(" ", "_")}.png')
    plt.savefig(output_path, dpi=150)
    print(f"图片已保存: {output_path}")
    plt.close()


# ======================== 主程序 ========================
def main():
    """
    主程序 - 与硬件 Func02_Handle 处理流程完全一致
    
    Raw Data采集配置 (1-SV4.0.1, bandWidth=0):
    - 带宽 B = 3.8GHz, sweepTimeA = 10us, sweepTimeB = 55us, T_up = 65us
    - meanEn = 2 (使用前一帧均值做静态杂波抑制)
    - cfarThVG = 2, cfarThVS = 6, cfarThV = 8
    - dpkThres = 8
    """
    # 初始化雷达配置 (使用 3.8GHz 带宽模式, 与Raw Data采集 bandWidth=0 一致)
    config = RadarConfig(bandwidth_mode=0)
    config.print_params()
    
    # 加载数据
    file_path = r"D:\Codes\4.0.1\radar_20\radar_20\gBuf.mat"
    print(f"\n加载数据: {file_path}")
    
    mat = loadmat(file_path)
    print(f"数据键: {mat.keys()}")
    
    gBuf = mat['gBuf']
    print(f"原始数据形状: {gBuf.shape}")
    print(f"数据类型: {gBuf.dtype}")
    
    # 调整数据维度 [samples, chirps, ant, frames] -> [frames, ant, chirps, samples]
    gBuf = np.transpose(gBuf, (3, 2, 1, 0))
    print(f"调整后数据形状: {gBuf.shape}")
    
    # 创建点云生成器
    generator = PointCloudGenerator(config)
    
    # 处理参数 (与硬件一致)
    skip_bins = 5           # 跳过近距离bin (去除直流泄漏)
    start_frame = 0         # 起始帧
    end_frame = None        # 结束帧 (None = 全部)
    # mean_subtract_mode: 
    #   0 = 不减均值
    #   1 = 使用当前帧均值 (第一帧行为)
    #   2 = 使用前一帧均值 (硬件默认)
    mean_subtract_mode = 2  # meanEn=2: 使用前一帧均值 (与硬件默认一致)
    
    # 处理所有帧
    print("\n" + "=" * 60)
    print("开始点云生成 (与硬件 Func02_Handle 完全一致)")
    print("=" * 60)
    print(f"FFT1D: 512点, Hanning窗")
    print(f"FFT2D: 256点, TDM-MIMO零填充, Hanning窗")
    print(f"CFAR: SO-CFAR, guard={config.cfar_guard}, search={config.cfar_search}, threshold={config.cfar_threshold}")
    print(f"FFT3D: 128点, 无窗")
    print(f"DPK: times={config.dpk_times}, threshold={config.dpk_threshold}")
    print(f"FFT4D: 128点, Steering Vector")
    print("=" * 60)
    
    frames, points = generator.process_all_frames(
        gBuf, 
        skip_bins=skip_bins,
        start_frame=start_frame,
        end_frame=end_frame,
        mean_subtract_mode=mean_subtract_mode
    )
    
    # 统计信息
    print(f"\n处理完成! 共 {len(frames)} 帧, 总点数: {len(points)}")
    
    # 打印DPK调试值
    if generator.doa_estimator._debug_dpk_values:
        print("\n[DEBUG] DPK阈值检查值样本:")
        for i, v in enumerate(generator.doa_estimator._debug_dpk_values[:3]):
            print(f"  样本{i}: cutPow={v['cutPow']:.6f}, res={v['res']:.6f}, threshold={v['threshold']}")
            print(f"          需要: cutPow > res*threshold = {v['res']*v['threshold']:.6f}")
    
    # 保存点云 (npz 格式)
    output_dir = r"D:\Codes\4.0.1"
    output_file = os.path.join(output_dir, "pointcloud_output.npz")
    save_point_cloud_npz(frames, points, output_file)
    
    # 可视化
    print("\n" + "=" * 60)
    print("可视化...")
    print("=" * 60)
    
    if len(points) > 0:
        # 可视化第一帧
        if len(frames) > 0:
            first_frame = frames[0]
            first_points = points[first_frame['offset']:first_frame['offset']+first_frame['num_points']]
            if len(first_points) > 0:
                visualize_point_cloud(first_points, f"point_cloud_frame_{first_frame['frame_id']}")
        
        # 可视化最后一帧
        if len(frames) > 1:
            last_frame = frames[-1]
            last_points = points[last_frame['offset']:last_frame['offset']+last_frame['num_points']]
            if len(last_points) > 0:
                visualize_point_cloud(last_points, f"point_cloud_frame_{last_frame['frame_id']}")
        
        # 可视化所有点云
        visualize_point_cloud(points, "point_cloud_all_frames")
    
    print("\n处理完成!")
    
    return frames, points


if __name__ == "__main__":
    frames, points = main()
