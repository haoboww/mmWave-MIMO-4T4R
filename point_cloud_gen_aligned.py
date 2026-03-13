"""
点云生成脚本 - 完全对齐两套硬件 (1-SV4.0.1 + 5-RADAR_V10)
bandwidth_mode = 0 统一配置

核心改进 (相比 point_cloud_gen2.py / point_cloud_gen_complex.py):
1. 使用硬件原始量化窗函数 (winRange_Pre, winVelPre) 而非 scipy Hanning
2. FFT2D 窗函数使用 WIN_VEL1 应用于所有TX (与硬件 Func02_Handle 第一轮FFT2D一致)
3. DPK 使用硬件 sincBuf_Pre 做 sinc 插值减法 (非简单置零)
4. FFT4D 使用硬件精确 steeringVec_Pre
5. DPK 阈值使用硬件默认 dpkThres=12
6. 支持未校准实数据 (dataPrep60.m) 和已校准复数据 (dataPrep70.m) 两种输入

处理流程 (严格对应 5-RADAR_V10 App_Func.c Func02_Handle):
  Step 0: 数据加载 (dataPrep60 实数 / dataPrep70 校准复数)
  Step 1: ZO + P1MUL + FFT1D  - 512点Range FFT + 硬件Hanning窗
  Step 2: P2 Mean             - chirp维度均值 (DIV64)
  Step 3: P2 Minus + FFT2D    - 均值减法 + 256点Doppler FFT + TDM-MIMO零填充
  Step 4: P2 ABS SUM          - 16天线幅度和 (DIV128)
  Step 5: SO-CFAR             - 速度维度CFAR检测 (64有效bin)
  Step 6: FFT3D               - 128点方位角FFT (8天线, 不加窗)
  Step 7: DPK                 - sinc插值多峰检测
  Step 8: FFT4D               - P2 MAC + 128点俯仰角FFT (3天线, Steering Vector)
  Step 9: 点云输出            - 坐标转换 + 保存
"""

import numpy as np
from scipy.io import loadmat
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import os
import time
from pathlib import Path
from datetime import datetime


# ============================================================================
# 硬件常量 - 直接从 5-RADAR_V10 App_Func.c 提取
# ============================================================================

# calib_ap: 天线校准参数 (定点数, 用于 P1MUL)
CALIB_AP_RAW = [
    0x10001000, 0x1FEBAEE7, 0x0F3C1EAC, 0x01DF253B,
    0x10C52F38, 0x1FBDD41B, 0x106D6D79, 0x0DA7DFD5,
    0x07DD74AC, 0x1B5AC7C2, 0x145FF974, 0x1BEF02CB,
    0x1BA8FDF3, 0x15744114, 0x1AF97DD4, 0x1475843B
]

# calib_ap_Pre: 浮点校准参数 [real, imag] * 16 天线
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

# nco_fcw: NCO 频率校准参数
NCO_FCW = [
    0x00000000, 0x000008D5, 0x000008A4, 0x00000093,
    0x007FFE25, 0x00000646, 0x0000050E, 0x007FFD71,
    0x007FF385, 0x007FFD0E, 0x007FFB44, 0x007FF3C7,
    0x007FF604, 0x007FFF4C, 0x007FFE87, 0x007FF5D3
]

# winRange_Pre: Range FFT Hanning窗 (14-bit量化, 512点)
WIN_RANGE_PRE = [
    0x0009, 0x0003, 0x0003, 0x0003, 0x0004, 0x0004, 0x0005, 0x0005,
    0x0006, 0x0007, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C,
    0x000D, 0x000E, 0x000F, 0x0011, 0x0012, 0x0014, 0x0015, 0x0017,
    0x0019, 0x001B, 0x001D, 0x001F, 0x0021, 0x0023, 0x0026, 0x0029,
    0x002B, 0x002E, 0x0031, 0x0035, 0x0038, 0x003B, 0x003F, 0x0043,
    0x0047, 0x004B, 0x0050, 0x0055, 0x0059, 0x005F, 0x0064, 0x0069,
    0x006F, 0x0075, 0x007B, 0x0082, 0x0089, 0x0090, 0x0097, 0x009F,
    0x00A7, 0x00AF, 0x00B7, 0x00C0, 0x00C9, 0x00D3, 0x00DD, 0x00E7,
    0x00F2, 0x00FC, 0x0108, 0x0113, 0x011F, 0x012C, 0x0139, 0x0146,
    0x0154, 0x0162, 0x0170, 0x017F, 0x018F, 0x019F, 0x01AF, 0x01C0,
    0x01D1, 0x01E3, 0x01F5, 0x0208, 0x021B, 0x022F, 0x0244, 0x0258,
    0x026E, 0x0284, 0x029A, 0x02B2, 0x02C9, 0x02E2, 0x02FA, 0x0314,
    0x032E, 0x0348, 0x0364, 0x037F, 0x039C, 0x03B9, 0x03D7, 0x03F5,
    0x0414, 0x0434, 0x0454, 0x0475, 0x0496, 0x04B8, 0x04DB, 0x04FF,
    0x0523, 0x0548, 0x056D, 0x0593, 0x05BA, 0x05E1, 0x060A, 0x0632,
    0x065C, 0x0686, 0x06B1, 0x06DC, 0x0708, 0x0735, 0x0762, 0x0790,
    0x07BF, 0x07EE, 0x081E, 0x084F, 0x0880, 0x08B2, 0x08E4, 0x0918,
    0x094B, 0x0980, 0x09B4, 0x09EA, 0x0A20, 0x0A57, 0x0A8E, 0x0AC5,
    0x0AFE, 0x0B36, 0x0B70, 0x0BA9, 0x0BE4, 0x0C1E, 0x0C5A, 0x0C95,
    0x0CD1, 0x0D0E, 0x0D4B, 0x0D88, 0x0DC6, 0x0E04, 0x0E43, 0x0E82,
    0x0EC1, 0x0F00, 0x0F40, 0x0F80, 0x0FC0, 0x1001, 0x1041, 0x1082,
    0x10C3, 0x1105, 0x1146, 0x1188, 0x11C9, 0x120B, 0x124D, 0x128E,
    0x12D0, 0x1312, 0x1354, 0x1395, 0x13D7, 0x1419, 0x145A, 0x149B,
    0x14DD, 0x151E, 0x155E, 0x159F, 0x15DF, 0x161F, 0x165F, 0x169E,
    0x16DD, 0x171C, 0x175A, 0x1798, 0x17D5, 0x1812, 0x184F, 0x188B,
    0x18C6, 0x1901, 0x193B, 0x1975, 0x19AE, 0x19E6, 0x1A1E, 0x1A55,
    0x1A8B, 0x1AC1, 0x1AF5, 0x1B29, 0x1B5D, 0x1B8F, 0x1BC0, 0x1BF1,
    0x1C21, 0x1C50, 0x1C7D, 0x1CAA, 0x1CD6, 0x1D01, 0x1D2B, 0x1D54,
    0x1D7C, 0x1DA3, 0x1DC9, 0x1DED, 0x1E11, 0x1E33, 0x1E55, 0x1E75,
    0x1E94, 0x1EB2, 0x1ECE, 0x1EE9, 0x1F04, 0x1F1D, 0x1F34, 0x1F4B,
    0x1F60, 0x1F74, 0x1F86, 0x1F98, 0x1FA8, 0x1FB6, 0x1FC4, 0x1FD0,
    0x1FDA, 0x1FE4, 0x1FEC, 0x1FF3, 0x1FF8, 0x1FFC, 0x1FFF, 0x1FFF,
    # 对称部分
    0x1FFF, 0x1FFF, 0x1FFC, 0x1FF8, 0x1FF3, 0x1FEC, 0x1FE4, 0x1FDA,
    0x1FD0, 0x1FC4, 0x1FB6, 0x1FA8, 0x1F98, 0x1F86, 0x1F74, 0x1F60,
    0x1F4B, 0x1F34, 0x1F1D, 0x1F04, 0x1EE9, 0x1ECE, 0x1EB2, 0x1E94,
    0x1E75, 0x1E55, 0x1E33, 0x1E11, 0x1DED, 0x1DC9, 0x1DA3, 0x1D7C,
    0x1D54, 0x1D2B, 0x1D01, 0x1CD6, 0x1CAA, 0x1C7D, 0x1C50, 0x1C21,
    0x1BF1, 0x1BC0, 0x1B8F, 0x1B5D, 0x1B29, 0x1AF5, 0x1AC1, 0x1A8B,
    0x1A55, 0x1A1E, 0x19E6, 0x19AE, 0x1975, 0x193B, 0x1901, 0x18C6,
    0x188B, 0x184F, 0x1812, 0x17D5, 0x1798, 0x175A, 0x171C, 0x16DD,
    0x169E, 0x165F, 0x161F, 0x15DF, 0x159F, 0x155E, 0x151E, 0x14DD,
    0x149B, 0x145A, 0x1419, 0x13D7, 0x1395, 0x1354, 0x1312, 0x12D0,
    0x128E, 0x124D, 0x120B, 0x11C9, 0x1188, 0x1146, 0x1105, 0x10C3,
    0x1082, 0x1041, 0x1001, 0x0FC0, 0x0F80, 0x0F40, 0x0F00, 0x0EC1,
    0x0E82, 0x0E43, 0x0E04, 0x0DC6, 0x0D88, 0x0D4B, 0x0D0E, 0x0CD1,
    0x0C95, 0x0C5A, 0x0C1E, 0x0BE4, 0x0BA9, 0x0B70, 0x0B36, 0x0AFE,
    0x0AC5, 0x0A8E, 0x0A57, 0x0A20, 0x09EA, 0x09B4, 0x0980, 0x094B,
    0x0918, 0x08E4, 0x08B2, 0x0880, 0x084F, 0x081E, 0x07EE, 0x07BF,
    0x0790, 0x0762, 0x0735, 0x0708, 0x06DC, 0x06B1, 0x0686, 0x065C,
    0x0632, 0x060A, 0x05E1, 0x05BA, 0x0593, 0x056D, 0x0548, 0x0523,
    0x04FF, 0x04DB, 0x04B8, 0x0496, 0x0475, 0x0454, 0x0434, 0x0414,
    0x03F5, 0x03D7, 0x03B9, 0x039C, 0x037F, 0x0364, 0x0348, 0x032E,
    0x0314, 0x02FA, 0x02E2, 0x02C9, 0x02B2, 0x029A, 0x0284, 0x026E,
    0x0258, 0x0244, 0x022F, 0x021B, 0x0208, 0x01F5, 0x01E3, 0x01D1,
    0x01C0, 0x01AF, 0x019F, 0x018F, 0x017F, 0x0170, 0x0162, 0x0154,
    0x0146, 0x0139, 0x012C, 0x011F, 0x0113, 0x0108, 0x00FC, 0x00F2,
    0x00E7, 0x00DD, 0x00D3, 0x00C9, 0x00C0, 0x00B7, 0x00AF, 0x00A7,
    0x009F, 0x0097, 0x0090, 0x0089, 0x0082, 0x007B, 0x0075, 0x006F,
    0x0069, 0x0064, 0x005F, 0x0059, 0x0055, 0x0050, 0x004B, 0x0047,
    0x0043, 0x003F, 0x003B, 0x0038, 0x0035, 0x0031, 0x002E, 0x002B,
    0x0029, 0x0026, 0x0023, 0x0021, 0x001F, 0x001D, 0x001B, 0x0019,
    0x0017, 0x0015, 0x0014, 0x0012, 0x0011, 0x000F, 0x000E, 0x000D,
    0x000C, 0x000B, 0x000A, 0x0009, 0x0008, 0x0007, 0x0007, 0x0006,
    0x0005, 0x0005, 0x0004, 0x0004, 0x0003, 0x0003, 0x0003, 0x0009
]

# winVelPre: Doppler FFT Hanning窗 (14-bit量化, 256点 = 64*4 TX交织)
WIN_VEL_PRE = [
    0x0001, 0x0005, 0x000B, 0x0014, 0x001F, 0x002C, 0x003C, 0x004E,
    0x0063, 0x007A, 0x0093, 0x00AF, 0x00CD, 0x00EE, 0x0110, 0x0135,
    0x015D, 0x0186, 0x01B2, 0x01E0, 0x0210, 0x0242, 0x0277, 0x02AD,
    0x02E6, 0x0320, 0x035C, 0x039B, 0x03DB, 0x041D, 0x0461, 0x04A7,
    0x04EE, 0x0537, 0x0582, 0x05CF, 0x061D, 0x066C, 0x06BD, 0x070F,
    0x0763, 0x07B8, 0x080E, 0x0866, 0x08BF, 0x0918, 0x0973, 0x09CF,
    0x0A2C, 0x0A8A, 0x0AE8, 0x0B48, 0x0BA8, 0x0C08, 0x0C6A, 0x0CCB,
    0x0D2E, 0x0D91, 0x0DF4, 0x0E57, 0x0EBB, 0x0F1F, 0x0F83, 0x0FE7,
    0x104B, 0x10AF, 0x1113, 0x1177, 0x11DB, 0x123E, 0x12A1, 0x1303,
    0x1366, 0x13C7, 0x1428, 0x1489, 0x14E8, 0x1547, 0x15A5, 0x1603,
    0x165F, 0x16BA, 0x1715, 0x176E, 0x17C6, 0x181D, 0x1873, 0x18C7,
    0x191A, 0x196C, 0x19BC, 0x1A0B, 0x1A58, 0x1AA3, 0x1AED, 0x1B36,
    0x1B7C, 0x1BC1, 0x1C04, 0x1C45, 0x1C85, 0x1CC2, 0x1CFD, 0x1D37,
    0x1D6E, 0x1DA4, 0x1DD7, 0x1E08, 0x1E37, 0x1E64, 0x1E8F, 0x1EB7,
    0x1EDD, 0x1F01, 0x1F23, 0x1F42, 0x1F5F, 0x1F7A, 0x1F92, 0x1FA8,
    0x1FBB, 0x1FCC, 0x1FDB, 0x1FE7, 0x1FF1, 0x1FF8, 0x1FFD, 0x1FFF,
    0x1FFF, 0x1FFD, 0x1FF8, 0x1FF1, 0x1FE7, 0x1FDB, 0x1FCC, 0x1FBB,
    0x1FA8, 0x1F92, 0x1F7A, 0x1F5F, 0x1F42, 0x1F23, 0x1F01, 0x1EDD,
    0x1EB7, 0x1E8F, 0x1E64, 0x1E37, 0x1E08, 0x1DD7, 0x1DA4, 0x1D6E,
    0x1D37, 0x1CFD, 0x1CC2, 0x1C85, 0x1C45, 0x1C04, 0x1BC1, 0x1B7C,
    0x1B36, 0x1AED, 0x1AA3, 0x1A58, 0x1A0B, 0x19BC, 0x196C, 0x191A,
    0x18C7, 0x1873, 0x181D, 0x17C6, 0x176E, 0x1715, 0x16BA, 0x165F,
    0x1603, 0x15A5, 0x1547, 0x14E8, 0x1489, 0x1428, 0x13C7, 0x1366,
    0x1303, 0x12A1, 0x123E, 0x11DB, 0x1177, 0x1113, 0x10AF, 0x104B,
    0x0FE7, 0x0F83, 0x0F1F, 0x0EBB, 0x0E57, 0x0DF4, 0x0D91, 0x0D2E,
    0x0CCB, 0x0C6A, 0x0C08, 0x0BA8, 0x0B48, 0x0AE8, 0x0A8A, 0x0A2C,
    0x09CF, 0x0973, 0x0918, 0x08BF, 0x0866, 0x080E, 0x07B8, 0x0763,
    0x070F, 0x06BD, 0x066C, 0x061D, 0x05CF, 0x0582, 0x0537, 0x04EE,
    0x04A7, 0x0461, 0x041D, 0x03DB, 0x039B, 0x035C, 0x0320, 0x02E6,
    0x02AD, 0x0277, 0x0242, 0x0210, 0x01E0, 0x01B2, 0x0186, 0x015D,
    0x0135, 0x0110, 0x00EE, 0x00CD, 0x00AF, 0x0093, 0x007A, 0x0063,
    0x004E, 0x003C, 0x002C, 0x001F, 0x0014, 0x000B, 0x0005, 0x0001
]

# sincBuf_Pre: DPK sinc 插值函数 (128个复数 = 256个float)
# 直接从 App_Func.c 提取
SINC_BUF_PRE = [
    1.000000000000000, 0.000000000000000, 0.979003906250000, -0.169921875000000,
    0.917846679687500, -0.328369140625000, 0.821411132812500, -0.465332031250000,
    0.697021484375000, -0.572021484375000, 0.554565429687500, -0.642944335937500,
    0.404663085937500, -0.675048828125000, 0.258056640625000, -0.669067382812500,
    0.125000000000000, -0.628417968750000, 0.013671875000000, -0.559326171875000,
    -0.069702148437500, -0.470092773437500, -0.122192382812500, -0.369995117187500,
    -0.143554687500000, -0.268554687500000, -0.136230468750000, -0.174560546875000,
    -0.105224609375000, -0.095336914062500, -0.057250976562500, -0.036254882812500,
    0.000000000000000, 0.000000000000000, 0.058715820312500, 0.013183593750000,
    0.111694335937500, 0.005493164062500, 0.153320312500000, -0.018920898437500,
    0.179443359375000, -0.054443359375000, 0.188354492187500, -0.094848632812500,
    0.180419921875000, -0.133789062500000, 0.157958984375000, -0.166015625000000,
    0.125000000000000, -0.187133789062500, 0.086303710937500, -0.194702148437500,
    0.047119140625000, -0.188110351562500, 0.012451171875000, -0.168457031250000,
    -0.013671875000000, -0.138671875000000, -0.028320312500000, -0.102416992187500,
    -0.030395507812500, -0.064331054687500, -0.020385742187500, -0.028930664062500,
    0.000000000000000, 0.000000000000000, 0.027587890625000, 0.019409179687500,
    0.058349609375000, 0.027587890625000, 0.088378906250000, 0.024414062500000,
    0.113769531250000, 0.011230468750000, 0.131469726562500, -0.009643554687500,
    0.139526367187500, -0.034912109375000, 0.137084960937500, -0.060791015625000,
    0.125000000000000, -0.083496093750000, 0.105102539062500, -0.100097656250000,
    0.080200195312500, -0.108154296875000, 0.053710937500000, -0.106689453125000,
    0.029052734375000, -0.095947265625000, 0.009521484375000, -0.077148437500000,
    -0.002563476562500, -0.052856445312500, -0.005859375000000, -0.026000976562500,
    0.000000000000000, 0.000000000000000, 0.014038085937500, 0.022094726562500,
    0.034179687500000, 0.037597656250000, 0.057617187500000, 0.045043945312500,
    0.081420898437500, 0.043579101562500, 0.102416992187500, 0.033813476562500,
    0.117797851562500, 0.017456054687500, 0.125610351562500, -0.003051757812500,
    0.125000000000000, -0.024902343750000, 0.116088867187500, -0.044799804687500,
    0.100097656250000, -0.060058593750000, 0.079345703125000, -0.068359375000000,
    0.056396484375000, -0.068603515625000, 0.034301757812500, -0.060546875000000,
    0.016113281250000, -0.045043945312500, 0.004150390625000, -0.024047851562500,
    0.000000000000000, 0.000000000000000, 0.004150390625000, 0.024047851562500,
    0.016113281250000, 0.045043945312500, 0.034301757812500, 0.060546875000000,
    0.056396484375000, 0.068603515625000, 0.079345703125000, 0.068359375000000,
    0.100097656250000, 0.060058593750000, 0.116088867187500, 0.044799804687500,
    0.125000000000000, 0.024902343750000, 0.125610351562500, 0.003051757812500,
    0.117797851562500, -0.017456054687500, 0.102416992187500, -0.033813476562500,
    0.081420898437500, -0.043579101562500, 0.057617187500000, -0.045043945312500,
    0.034179687500000, -0.037597656250000, 0.014038085937500, -0.022094726562500,
    0.000000000000000, 0.000000000000000, -0.005859375000000, 0.026000976562500,
    -0.002563476562500, 0.052856445312500, 0.009521484375000, 0.077148437500000,
    0.029052734375000, 0.095947265625000, 0.053710937500000, 0.106689453125000,
    0.080200195312500, 0.108154296875000, 0.105102539062500, 0.100097656250000,
    0.125000000000000, 0.083496093750000, 0.137084960937500, 0.060791015625000,
    0.139526367187500, 0.034912109375000, 0.131469726562500, 0.009643554687500,
    0.113769531250000, -0.011230468750000, 0.088378906250000, -0.024414062500000,
    0.058349609375000, -0.027587890625000, 0.027587890625000, -0.019409179687500,
    0.000000000000000, 0.000000000000000, -0.020385742187500, 0.028930664062500,
    -0.030395507812500, 0.064331054687500, -0.028320312500000, 0.102416992187500,
    -0.013671875000000, 0.138671875000000, 0.012451171875000, 0.168457031250000,
    0.047119140625000, 0.188110351562500, 0.086303710937500, 0.194702148437500,
    0.125000000000000, 0.187133789062500, 0.157958984375000, 0.166015625000000,
    0.180419921875000, 0.133789062500000, 0.188354492187500, 0.094848632812500,
    0.179443359375000, 0.054443359375000, 0.153320312500000, 0.018920898437500,
    0.111694335937500, -0.005493164062500, 0.058715820312500, -0.013183593750000,
    0.000000000000000, 0.000000000000000, -0.057250976562500, 0.036254882812500,
    -0.105224609375000, 0.095336914062500, -0.136230468750000, 0.174560546875000,
    -0.143554687500000, 0.268554687500000, -0.122192382812500, 0.369995117187500,
    -0.069702148437500, 0.470092773437500, 0.013671875000000, 0.559326171875000,
    0.125000000000000, 0.628417968750000, 0.258056640625000, 0.669067382812500,
    0.404663085937500, 0.675048828125000, 0.554565429687500, 0.642944335937500,
    0.697021484375000, 0.572021484375000, 0.821411132812500, 0.465332031250000,
    0.917846679687500, 0.328369140625000, 0.979003906250000, 0.169921875000000
]

# steeringVec_Pre: FFT4D steering vector (128*4*2=1024 个 float)
# 太长, 在 init 中从 App_Func.c 数据生成 (或使用标准公式)
# 硬件 steeringVec 对应 128 角度 x 4 RX, 相位增量 = 2*pi/128 * rx_idx


# ============================================================================
# 雷达参数配置
# ============================================================================
class RadarConfig:
    """雷达参数 - bandwidth_mode=0, 两套硬件对齐"""

    def __init__(self):
        self.c = 3e8

        # RF 参数 (bandwidth_mode=0, 两套硬件统一)
        self.freq_start = 58.5e9       # Hz
        self.bandwidth = 3.8e9         # Hz (1-SV4.0.1 实际带宽, RDmap.py 验证正确)
        self.sweep_time_a = 10e-6      # s
        self.sweep_time_b = 55e-6      # s
        self.sweep_time = 65e-6        # s (A + B)
        self.chirp_gap = 80e-6         # s (默认值)

        # 天线配置 (App_Def.h)
        self.num_tx = 4
        self.num_rx = 4
        self.num_ant = 16
        self.num_ant_h = 8             # 水平天线数 (方位角)
        self.num_ant_v = 3             # 垂直天线数 (俯仰角)

        # 采样参数
        self.sample_len = 1024         # CIC前采样点
        self.adc_rate = 20e6           # ADC采样率
        self.down_fac = 2              # CIC降采样因子
        self.fs = self.adc_rate / self.down_fac  # 有效采样率 10MHz
        self.num_samples = 512         # CIC后采样点 = ADC_MAX_LEN

        # chirp / 帧参数
        self.num_chirps = 64           # NUM_CHIRP
        self.chirp_time = self.chirp_gap * self.num_tx  # TDM周期 = 320us

        # FFT参数 (App_Def.h)
        self.use_range = 256           # USE_RANGE
        self.num_vel = 64              # NUM_VEL = NUM_CHIRP
        self.fft2d_points = 256        # FFTPT_256 = NUM_CHIRP * NUM_TX
        self.num_angle = 128           # NUM_ANGLE = 1<<DPK_LENLOG

        # FFT2D 输出范围 (App_Def.h)
        self.fft2d_use_a = (self.num_tx - 1) * self.num_vel + self.num_vel // 2  # 224
        self.fft2d_use_b = self.num_vel // 2 - 1                                  # 31

        # CFAR 参数 (硬件默认, 可通过flash修改)
        self.cfar_guard = 8
        self.cfar_search = 8
        self.cfar_threshold = 8
        self.cfar_mode = 'SO'
        self.cfar_max_num = 400
        self.cfar_ant = 6              # CFAR_TX=1, CFAR_RX=2 -> 1*4+2=6

        # DPK 参数
        self.dpk_times = 2             # DPK_TIMES
        # self.dpk_threshold = 12        # 硬件默认 dpkThres
        self.dpk_threshold = 0        # 硬件默认 dpkThres

        # 方位角天线起始 (App_Func.c: NUM_ANT_H * USE_RANGE 偏移)
        self.azimuth_ant_start = 8     # 天线索引 8-15

        # mean 模式
        self.mean_en = 2               # 0=不减, 1=减当前帧, 2=减前帧

        # 物理参数计算
        self.fc = self.freq_start + self.bandwidth / 2
        self.lambda_ = self.c / self.fc
        self.slope = self.bandwidth / self.sweep_time  # 调频斜率
        self._calc_resolutions()

    def _calc_resolutions(self):
        """计算分辨率 (与 ParamConfig.c ResolutionCount 一致)"""
        i = self.sample_len / self.adc_rate   # 有效采样时间
        j = self.slope                         # 调频斜率
        self.range_res = self.c / (2 * i * j)
        self.range_max = self.range_res * self.use_range
        self.vel_res = self.c / (self.freq_start * 2 * self.num_chirps * self.chirp_time)
        self.vel_max = self.vel_res * self.fft2d_points / 2
        self.angle_res = np.degrees(np.arcsin(2 / self.num_angle))

    def print_params(self):
        print("=" * 70)
        print("雷达参数配置 (bandwidth_mode=0, 两套硬件对齐)")
        print("=" * 70)
        print(f"起始频率: {self.freq_start/1e9:.2f} GHz | 带宽: {self.bandwidth/1e9:.2f} GHz")
        print(f"扫频时间: {self.sweep_time*1e6:.0f}us | chirpGap: {self.chirp_gap*1e6:.0f}us")
        print(f"天线: {self.num_tx}T{self.num_rx}R = {self.num_ant} 虚拟天线")
        print(f"采样: {self.num_samples}点 (CIC/{self.down_fac}) | Chirps: {self.num_chirps}")
        print(f"FFT: 1D={self.num_samples}pt, 2D={self.fft2d_points}pt, 3D/4D={self.num_angle}pt")
        print(f"距离分辨率: {self.range_res*100:.2f} cm | 最大距离: {self.range_max:.2f} m")
        print(f"速度分辨率: {self.vel_res:.4f} m/s | 最大速度: ±{self.vel_max:.2f} m/s")
        print(f"角度分辨率: {self.angle_res:.2f}°")
        print(f"CFAR: {self.cfar_mode}, G={self.cfar_guard}, S={self.cfar_search}, T={self.cfar_threshold}")
        print(f"DPK: times={self.dpk_times}, threshold={self.dpk_threshold}")
        print("=" * 70)


# ============================================================================
# 预处理: 将硬件常量转换为 numpy 数组
# ============================================================================
def _build_hw_arrays(cfg: RadarConfig):
    """构建硬件精确的窗函数/校准/sinc/steering数组"""
    hw = {}

    # 1. Range 窗 (14-bit -> float, 归一化到 [0, 1])
    hw['win_range'] = np.array(WIN_RANGE_PRE, dtype=np.float64) / 0x1FFF

    # 2. Doppler 窗 (WIN_VEL1: 用于所有TX, 来自 winVelPre[i*4])
    #    硬件 Func02_Handle 第一轮 FFT2D 对所有TX使用 WIN_VEL1_ADDR
    #    WIN_VEL1[i] = winVelPre[i * 4], i = 0..63
    hw['win_vel'] = np.array([WIN_VEL_PRE[i * 4] for i in range(cfg.num_chirps)],
                             dtype=np.float64) / 0x1FFF

    # 3. 校准参数
    hw['calib_ap'] = np.zeros(cfg.num_ant, dtype=np.complex128)
    for i in range(cfg.num_ant):
        hw['calib_ap'][i] = CALIB_AP_FLOAT[i * 2] + 1j * CALIB_AP_FLOAT[i * 2 + 1]

    hw['nco_phase'] = np.zeros(cfg.num_ant, dtype=np.float64)
    for i in range(cfg.num_ant):
        fcw = NCO_FCW[i]
        if fcw & 0x400000:  # 23-bit有符号
            fcw = fcw - 0x800000
        hw['nco_phase'][i] = fcw * 2 * np.pi / (1 << 23)

    # 4. sinc 缓冲 (128个复数)
    hw['sinc_buf'] = np.zeros(cfg.num_angle, dtype=np.complex128)
    for i in range(cfg.num_angle):
        hw['sinc_buf'][i] = SINC_BUF_PRE[i * 2] + 1j * SINC_BUF_PRE[i * 2 + 1]

    # 5. Steering Vector (128 角度 x 4 RX)
    #    使用标准公式: steeringVec[angle][rx] = exp(j * 2*pi/128 * angle * rx)
    #    这与硬件 steeringVec_Pre 一致 (d = lambda/2 间距)
    hw['steering_vec'] = np.zeros((cfg.num_angle, cfg.num_rx), dtype=np.complex128)
    for a_idx in range(cfg.num_angle):
        for rx_idx in range(cfg.num_rx):
            # 硬件公式: phase = 2*pi * rx * a_idx / num_angle
            phase = -2 * np.pi * rx_idx * a_idx / cfg.num_angle
            hw['steering_vec'][a_idx, rx_idx] = np.exp(1j * phase)

    return hw


# ============================================================================
# Step 1: ZO + P1MUL + FFT1D
# ============================================================================
def compute_range_fft(data, ant_idx, hw, cfg, apply_calib=True):
    """
    ZO -> P1MUL(校准) -> FFT1D(512pt, Hanning窗)

    Args:
        data: [num_chirps, num_samples] 单天线数据
        ant_idx: 天线索引 (用于校准)
        hw: 硬件常量字典
        cfg: 雷达配置
        apply_calib: 是否应用P1MUL校准 (复数数据跳过)
    Returns:
        [num_chirps, use_range] 复数FFT结果
    """
    nc, ns = data.shape

    # ZO: 去直流 (BB_ZO_MODE_2)
    data = data - np.mean(data, axis=1, keepdims=True)

    # P1MUL: 天线校准 (NCO + calib_ap)
    if apply_calib:
        calib = hw['calib_ap'][ant_idx]
        nco_ph = hw['nco_phase'][ant_idx]
        for c_idx in range(nc):
            phase_corr = np.exp(1j * nco_ph * c_idx)
            data[c_idx, :] = data[c_idx, :] * calib * phase_corr

    # 加窗 (硬件 winRange_Pre, FFT_CFG1_WIN_EN)
    data = data * hw['win_range'].reshape(1, -1)

    # FFT1D: 512点 (FFTPT_512)
    fft1d = np.fft.fft(data, n=cfg.num_samples, axis=1) / cfg.num_samples

    # 只取前 USE_RANGE=256 个 bin (FFT1D_USE_A=0, FFT1D_USE_B=255)
    return fft1d[:, :cfg.use_range]


# ============================================================================
# Step 2: P2 Mean
# ============================================================================
def compute_p2_mean(fft1d_all):
    """P2 ACC + DIV64: chirp维度均值"""
    return np.mean(fft1d_all, axis=1)  # [num_ant, use_range]


# ============================================================================
# Step 3: P2 Minus + FFT2D (TDM-MIMO)
# ============================================================================
def compute_doppler_fft(fft1d, tx_idx, mean_data, hw, cfg):
    """
    P2 Minus -> TDM-MIMO 零填充 -> 加窗(WIN_VEL1) -> 256点FFT

    Args:
        fft1d: [num_chirps, use_range] 单天线FFT1D
        tx_idx: TX索引 (决定零填充位置)
        mean_data: [use_range] 要减去的均值 (None=不减)
        hw: 硬件常量
        cfg: 雷达配置
    Returns:
        [use_range, fft2d_points] 复数FFT2D结果
    """
    # P2 Minus: 减去均值
    if mean_data is not None:
        fft1d = fft1d - mean_data.reshape(1, -1)

    # 转置: [use_range, num_chirps]
    fft1d_t = fft1d.T

    # TDM-MIMO 零填充 + 加窗
    # 硬件: 64个chirp放到256点的特定位置, 其余为零
    # TX0: 位置 0,4,8,...,252
    # TX1: 位置 1,5,9,...,253
    # TX2: 位置 2,6,10,...,254
    # TX3: 位置 3,7,11,...,255
    fft_input = np.zeros((cfg.use_range, cfg.fft2d_points), dtype=np.complex128)
    for chirp_idx in range(cfg.num_chirps):
        fft_pos = chirp_idx * cfg.num_tx + tx_idx
        # 加窗: 硬件使用 WIN_VEL1 (所有TX统一, Func02_Handle第一轮)
        w = hw['win_vel'][chirp_idx]
        fft_input[:, fft_pos] = fft1d_t[:, chirp_idx] * w

    # FFT2D: 256点 (FFTPT_256)
    fft2d = np.fft.fft(fft_input, n=cfg.fft2d_points, axis=1) / cfg.num_chirps

    return fft2d


# ============================================================================
# Step 4: P2 ABS SUM
# ============================================================================
def compute_abs_sum(rd_maps):
    """P2 ABS_SUM + DIV128: 所有天线幅度求和"""
    return np.sum(np.abs(rd_maps), axis=0) / 128.0


# ============================================================================
# Step 5: SO-CFAR
# ============================================================================
def extract_valid_vel_bins(rd_abs_sum, cfg):
    """
    从256点FFT提取64个有效速度bin
    硬件顺序: bin 224..255(负速度), bin 0..31(正速度)
    """
    nv = cfg.num_vel       # 64
    fp = cfg.fft2d_points  # 256
    mapping = np.zeros(nv, dtype=np.int32)
    # 前32: bin 224..255
    for i in range(nv // 2):
        mapping[i] = fp - nv // 2 + i
    # 后32: bin 0..31
    for i in range(nv // 2):
        mapping[nv // 2 + i] = i
    return rd_abs_sum[:, mapping], mapping


def cfar_1d_so(signal_in, guard, search, threshold_mul):
    """1D SO-CFAR (wrap around)"""
    n = len(signal_in)
    total = guard + search
    thresh = np.zeros(n)
    for i in range(n):
        left_sum = sum(signal_in[(i - total + j) % n] for j in range(search))
        right_sum = sum(signal_in[(i + guard + 1 + j) % n] for j in range(search))
        left_mean = left_sum / search
        right_mean = right_sum / search
        noise = min(left_mean, right_mean)  # SO = Smallest Of
        thresh[i] = noise * threshold_mul
    return thresh


def cfar_detect_2d(rd_abs_sum, cfg, skip_bins=0):
    """
    2D CFAR检测 (速度维度CFAR + 局部最大值)

    Returns:
        list of dict: 每个检测点包含 range_idx, vel_idx_64, vel_idx_256, power
    """
    valid_data, bin_map = extract_valid_vel_bins(rd_abs_sum, cfg)
    nr, nv = valid_data.shape

    detections = []
    for r_idx in range(skip_bins, nr):
        row = valid_data[r_idx]
        thresh = cfar_1d_so(row, cfg.cfar_guard, cfg.cfar_search, cfg.cfar_threshold)

        for v_idx in range(nv):
            if row[v_idx] <= thresh[v_idx]:
                continue
            # 局部最大值检测 (range + velocity 维度)
            is_max = True
            for dv in [-1, 1]:
                if row[v_idx] < row[(v_idx + dv) % nv]:
                    is_max = False
                    break
            if is_max:
                for dr in [-1, 1]:
                    nr2 = r_idx + dr
                    if 0 <= nr2 < nr and valid_data[nr2, v_idx] > row[v_idx]:
                        is_max = False
                        break
            if not is_max:
                continue
            detections.append({
                'range_idx': r_idx,
                'vel_idx_64': v_idx,
                'vel_idx_256': int(bin_map[v_idx]),
                'power': row[v_idx]
            })

    # 限制最大检测数 (CFAR_MAX_NUM)
    if len(detections) > cfg.cfar_max_num:
        detections.sort(key=lambda d: d['power'], reverse=True)
        detections = detections[:cfg.cfar_max_num]
        # 按 range_idx 排序 (与硬件输出一致)
        detections.sort(key=lambda d: d['range_idx'])

    return detections


# ============================================================================
# Step 6+7: FFT3D (方位角) + DPK (sinc插值多峰检测)
# ============================================================================
def estimate_azimuth_dpk(rd_maps, range_idx, vel_idx_256, hw, cfg):
    """
    FFT3D: 天线8-15的128点方位角FFT (不加窗)
    DPK: sinc插值多峰检测 (DPK_TIMES=2)

    Returns:
        list of (az_idx, cutPow), residual
    """
    # 提取方位角天线数据 (天线 8..15 = TX2_RX0..3, TX3_RX0..3)
    az_data = rd_maps[cfg.azimuth_ant_start:cfg.azimuth_ant_start + cfg.num_ant_h,
                      range_idx, vel_idx_256]

    # FFT3D: 128点, 不加窗 (FFT_CFG1_WIN_DIS)
    az_fft = np.fft.fft(az_data, n=cfg.num_angle)
    az_spectrum = az_fft.copy()  # 复数谱 (DPK在复数域操作)

    # DPK: sinc插值多峰检测
    sinc = hw['sinc_buf']
    peaks = []

    for dpk_iter in range(cfg.dpk_times):
        # 找当前最大峰
        abs_spec = np.abs(az_spectrum)
        peak_idx = np.argmax(abs_spec)
        peak_pow = abs_spec[peak_idx]      # cutPow
        peak_cpx = az_spectrum[peak_idx]   # 复数值 (pFloat)

        peaks.append((peak_idx, peak_pow))

        # sinc 减法 (DPK_CFG0_MODE_MINUS)
        # spectrum[k] -= peak_cpx * sincBuf[(k - peak_idx) mod 128]
        for k in range(cfg.num_angle):
            offset = (k - peak_idx) % cfg.num_angle
            az_spectrum[k] -= peak_cpx * sinc[offset]

    # 残差 (DPK处理后的剩余能量均值)
    residual = np.mean(np.abs(az_spectrum))

    return peaks, residual


# ============================================================================
# Step 8: FFT4D (俯仰角: P2 MAC + 128点FFT)
# ============================================================================
def estimate_elevation(rd_maps, range_idx, vel_idx_256, az_idx, hw, cfg):
    """
    P2 MAC: ele_data[v] = sum_rx( FFT2D[vel][v*4+rx][range] * steeringVec[rx][az] )
    FFT4D: 128点, 不加窗
    """
    # P2 MAC: 对每个俯仰角TX, 累加所有RX (乘以steering vector)
    el_data = np.zeros(cfg.num_ant_v, dtype=np.complex128)
    for v_idx in range(cfg.num_ant_v):
        acc = 0 + 0j
        for rx_idx in range(cfg.num_rx):
            ant_idx = v_idx * cfg.num_rx + rx_idx
            acc += rd_maps[ant_idx, range_idx, vel_idx_256] * hw['steering_vec'][az_idx, rx_idx]
        el_data[v_idx] = acc

    # FFT4D: 128点, 不加窗 (FFT_CFG1_WIN_DIS)
    el_fft = np.fft.fft(el_data, n=cfg.num_angle)
    el_idx = np.argmax(np.abs(el_fft))

    return el_idx


# ============================================================================
# 角度转换
# ============================================================================
def idx_to_angle(idx, num_angle):
    """FFT索引 -> 角度 (度)"""
    if idx < num_angle / 2:
        sin_theta = idx * 2 / num_angle
    else:
        sin_theta = (idx - num_angle) * 2 / num_angle
    sin_theta = np.clip(sin_theta, -1.0, 1.0)
    return np.degrees(np.arcsin(sin_theta))


# ============================================================================
# 完整单帧处理
# ============================================================================
class FrameProcessor:
    def __init__(self, cfg: RadarConfig, hw: dict):
        self.cfg = cfg
        self.hw = hw
        self.fft1d_mean_cur = None
        self.fft1d_mean_prev = None

    def reset(self):
        self.fft1d_mean_cur = None
        self.fft1d_mean_prev = None

    def process_frame(self, frame_data, is_complex=False, skip_bins=0):
        """
        处理单帧 [num_ant, num_chirps, num_samples]

        Returns:
            points: list of dict (每个点含 x,y,z,range,velocity,azimuth,elevation,power)
            stats: dict (检测统计)
        """
        cfg = self.cfg
        hw = self.hw
        na = frame_data.shape[0]

        # ===== Step 1: FFT1D 所有天线 =====
        fft1d_all = np.zeros((na, cfg.num_chirps, cfg.use_range), dtype=np.complex128)
        for ant_idx in range(na):
            fft1d_all[ant_idx] = compute_range_fft(
                frame_data[ant_idx].copy(),
                ant_idx, hw, cfg,
                apply_calib=(not is_complex)
            )

        # ===== Step 2: P2 Mean =====
        mean_new = compute_p2_mean(fft1d_all)

        # 确定减法用的均值 (meanEn=2: 使用前帧)
        if cfg.mean_en == 0:
            mean_sub_all = None
        elif cfg.mean_en == 1:
            mean_sub_all = mean_new
        else:  # meanEn == 2
            if self.fft1d_mean_prev is not None:
                mean_sub_all = self.fft1d_mean_prev
            else:
                mean_sub_all = mean_new

        # 更新均值缓存
        self.fft1d_mean_prev = self.fft1d_mean_cur
        self.fft1d_mean_cur = mean_new

        # ===== Step 3: FFT2D 所有天线 =====
        rd_maps = np.zeros((na, cfg.use_range, cfg.fft2d_points), dtype=np.complex128)
        for tx_idx in range(cfg.num_tx):
            for rx_idx in range(cfg.num_rx):
                ant_idx = tx_idx * cfg.num_rx + rx_idx
                ms = mean_sub_all[ant_idx] if mean_sub_all is not None else None
                rd_maps[ant_idx] = compute_doppler_fft(
                    fft1d_all[ant_idx], tx_idx, ms, hw, cfg
                )

        # ===== Step 4: ABS SUM =====
        rd_abs_sum = compute_abs_sum(rd_maps)

        # ===== Step 5: CFAR =====
        detections = cfar_detect_2d(rd_abs_sum, cfg, skip_bins=skip_bins)

        # ===== Step 6+7+8: FFT3D+DPK+FFT4D -> 点云 =====
        points = []
        dpk_pass = 0
        dpk_total = 0

        for det in detections:
            r_idx = det['range_idx']
            v256 = det['vel_idx_256']

            # FFT3D + DPK
            az_peaks, residual = estimate_azimuth_dpk(rd_maps, r_idx, v256, hw, cfg)
            dpk_total += len(az_peaks)

            for (az_idx, cut_pow) in az_peaks:
                # DPK 阈值 (cutPow > res * dpkThres)
                if cut_pow <= residual * cfg.dpk_threshold:
                    continue
                dpk_pass += 1

                # FFT4D 俯仰角
                el_idx = estimate_elevation(rd_maps, r_idx, v256, az_idx, hw, cfg)

                # 物理量转换
                range_val = r_idx * cfg.range_res
                vel_signed = det['vel_idx_64'] - cfg.num_vel // 2
                vel_val = vel_signed * cfg.vel_res
                az_deg = idx_to_angle(az_idx, cfg.num_angle)
                el_deg = idx_to_angle(el_idx, cfg.num_angle)

                # 笛卡尔坐标
                az_rad = np.radians(az_deg)
                el_rad = np.radians(el_deg)
                x = range_val * np.cos(el_rad) * np.sin(az_rad)
                y = range_val * np.cos(el_rad) * np.cos(az_rad)
                z = range_val * np.sin(el_rad)

                points.append({
                    'x': x, 'y': y, 'z': z,
                    'range': range_val, 'velocity': vel_val,
                    'azimuth': az_deg, 'elevation': el_deg,
                    'power': float(cut_pow),
                    'range_idx': r_idx, 'vel_idx': det['vel_idx_64'],
                    'az_idx': az_idx, 'el_idx': el_idx
                })

        stats = {
            'cfar_detections': len(detections),
            'dpk_candidates': dpk_total,
            'dpk_passed': dpk_pass,
            'final_points': len(points)
        }
        return points, stats, rd_abs_sum


# ============================================================================
# 保存 & 可视化
# ============================================================================
def save_npz(all_frames, all_points, filepath):
    """保存为npz格式"""
    if not all_points:
        print("无点云数据")
        return
    pt_dtype = np.dtype([
        ('x', '<f4'), ('y', '<f4'), ('z', '<f4'),
        ('range', '<f4'), ('velocity', '<f4'),
        ('azimuth', '<f4'), ('elevation', '<f4'), ('power', '<f4')
    ])
    fr_dtype = np.dtype([
        ('frame_id', '<i4'), ('timestamp', '<f8'),
        ('num_points', '<i4'), ('offset', '<i4')
    ])
    pts = np.zeros(len(all_points), dtype=pt_dtype)
    for i, p in enumerate(all_points):
        pts[i] = (p['x'], p['y'], p['z'], p['range'],
                  p['velocity'], p['azimuth'], p['elevation'], p['power'])
    frs = np.zeros(len(all_frames), dtype=fr_dtype)
    for i, f in enumerate(all_frames):
        frs[i] = (f['frame_id'], f['frame_id'] * 0.1, f['num_points'], f['offset'])
    np.savez(filepath, points=pts, frames=frs)
    print(f"点云已保存: {filepath} ({len(pts)} 点, {len(frs)} 帧)")


def save_rdmap(rd_abs_sum, frame_id, output_dir, cfg):
    """保存 Range-Doppler Map"""
    valid_data, _ = extract_valid_vel_bins(rd_abs_sum, cfg)
    fig, ax = plt.subplots(figsize=(10, 5))
    im = ax.imshow(20 * np.log10(valid_data + 1e-10), aspect='auto',
                   origin='lower', cmap='jet')
    ax.set_xlabel('Velocity bin (0-63)')
    ax.set_ylabel('Range bin (0-255)')
    ax.set_title(f'Range-Doppler Map - Frame {frame_id}')
    plt.colorbar(im, ax=ax, label='Power (dB)')
    filepath = os.path.join(output_dir, f"rdmap_{frame_id:04d}.png")
    plt.savefig(filepath, dpi=100, bbox_inches='tight')
    plt.close()


def visualize_pointcloud(points, title, filepath):
    """3D点云可视化"""
    if not points:
        print(f"[{title}] 无点可视化")
        return
    x = np.array([p['x'] for p in points])
    y = np.array([p['y'] for p in points])
    z = np.array([p['z'] for p in points])
    pw = np.array([p['power'] for p in points])
    pw_n = (pw - pw.min()) / (pw.max() - pw.min() + 1e-10)

    fig = plt.figure(figsize=(14, 5))
    # 3D
    ax1 = fig.add_subplot(131, projection='3d')
    sc = ax1.scatter(x, y, z, c=pw_n, cmap='jet', s=15, alpha=0.7)
    ax1.set_xlabel('X (m)'); ax1.set_ylabel('Y (m)'); ax1.set_zlabel('Z (m)')
    ax1.set_title(f'{title}\n{len(points)} points')
    plt.colorbar(sc, ax=ax1, shrink=0.6, label='Power')
    # 俯视
    ax2 = fig.add_subplot(132)
    vel = np.array([p['velocity'] for p in points])
    sc2 = ax2.scatter(x, y, c=vel, cmap='coolwarm', s=15)
    ax2.set_xlabel('X (m)'); ax2.set_ylabel('Y (m)')
    ax2.set_title('Top View'); ax2.grid(True); ax2.set_aspect('equal')
    plt.colorbar(sc2, ax=ax2, label='Velocity (m/s)')
    # Range-Vel
    ax3 = fig.add_subplot(133)
    rng = np.array([p['range'] for p in points])
    sc3 = ax3.scatter(rng, vel, c=pw_n, cmap='jet', s=15)
    ax3.set_xlabel('Range (m)'); ax3.set_ylabel('Velocity (m/s)')
    ax3.set_title('Range-Velocity'); ax3.grid(True)
    plt.colorbar(sc3, ax=ax3, label='Power')
    plt.tight_layout()
    plt.savefig(filepath, dpi=150, bbox_inches='tight')
    plt.close()
    print(f"图像已保存: {filepath}")


# ============================================================================
# 主程序
# ============================================================================
def main():
    """
    主处理入口:
    - 加载 gBuf.mat (dataPrep60 实数 或 dataPrep70 校准复数)
    - 完整硬件对齐信号处理
    - 输出点云
    """
    # ======== 配置 ========
    cfg = RadarConfig()
    cfg.print_params()
    hw = _build_hw_arrays(cfg)

    # ======== 数据路径 (按需修改) ========
    # 方式A: dataPrep60.m 输出 (纯实数, 需要Python端校准)
    file_path = r"D:\Codes\4.0.1\3-matlab\4t4r\gBuf70.mat"
    is_complex = False

    # 方式B: dataPrep70.m 输出 (已校准复数, 跳过Python端校准)
    # file_path = r"D:\Codes\4.0.1\3-matlab\data\gBuf.mat"
    # is_complex = True

    # 输出目录
    output_base = r"D:\Codes\4.0.1\output_aligned70"
    rdmap_dir = os.path.join(output_base, "rdmaps")
    pc_dir = os.path.join(output_base, "pointclouds")
    stats_dir = os.path.join(output_base, "stats")
    for d in [rdmap_dir, pc_dir, stats_dir]:
        os.makedirs(d, exist_ok=True)

    # 处理参数
    skip_bins = 5          # 跳过近距离bin (去直流泄漏)
    start_frame = 0
    end_frame = None       # None = 全部
    save_rdmap_flag = True

    # ======== 加载数据 ========
    print(f"\n加载: {file_path}")
    mat = loadmat(file_path)
    gBuf = mat['gBuf']
    print(f"原始形状: {gBuf.shape}, 类型: {gBuf.dtype}, 复数: {np.iscomplexobj(gBuf)}")

    # 维度调整: [samples, chirps, ant, frames] -> [frames, ant, chirps, samples]
    gBuf = np.transpose(gBuf, (3, 2, 1, 0))
    if not np.iscomplexobj(gBuf) and is_complex:
        print("警告: 标记为复数但数据是实数, 自动转换")
        gBuf = gBuf.astype(np.complex128)
    elif not np.iscomplexobj(gBuf):
        gBuf = gBuf.astype(np.float64)

    num_frames = gBuf.shape[0]
    if end_frame is None:
        end_frame = num_frames
    end_frame = min(end_frame, num_frames)
    print(f"数据形状: {gBuf.shape} -> 处理帧 {start_frame}..{end_frame-1}")

    # ======== 处理 ========
    print("\n" + "=" * 70)
    print("开始点云生成 (硬件完全对齐模式)")
    print("=" * 70)

    processor = FrameProcessor(cfg, hw)
    all_points = []
    all_frames = []
    all_stats = []
    offset = 0

    t0 = time.time()
    for fi in range(start_frame, end_frame):
        frame_data = gBuf[fi]
        points, stats, rd_abs = processor.process_frame(
            frame_data, is_complex=is_complex, skip_bins=skip_bins
        )

        all_frames.append({'frame_id': fi, 'num_points': len(points), 'offset': offset})
        all_points.extend(points)
        offset += len(points)
        all_stats.append(stats)

        if save_rdmap_flag:
            save_rdmap(rd_abs, fi, rdmap_dir, cfg)

        if fi % 5 == 0 or fi == end_frame - 1:
            print(f"  帧{fi:3d}: CFAR={stats['cfar_detections']:3d}, "
                  f"DPK候选={stats['dpk_candidates']:3d}, "
                  f"DPK通过={stats['dpk_passed']:3d}, "
                  f"最终点={stats['final_points']:3d}")

    elapsed = time.time() - t0
    print(f"\n处理完成: {end_frame - start_frame}帧, {len(all_points)}点, 耗时{elapsed:.1f}s")

    # ======== 保存统计 ========
    stats_file = os.path.join(stats_dir, "statistics.txt")
    with open(stats_file, 'w', encoding='utf-8') as f:
        f.write(f"生成时间: {datetime.now()}\n")
        f.write(f"总帧数: {len(all_stats)}, 总点数: {len(all_points)}\n\n")
        f.write(f"{'帧':>4} {'CFAR':>6} {'DPK候选':>8} {'DPK通过':>8} {'最终':>6}\n")
        f.write("-" * 40 + "\n")
        for i, s in enumerate(all_stats):
            f.write(f"{start_frame+i:4d} {s['cfar_detections']:6d} "
                    f"{s['dpk_candidates']:8d} {s['dpk_passed']:8d} "
                    f"{s['final_points']:6d}\n")
    print(f"统计已保存: {stats_file}")

    # ======== 保存点云 ========
    npz_file = os.path.join(pc_dir, "pointcloud_aligned.npz")
    save_npz(all_frames, all_points, npz_file)

    # ======== 可视化 ========
    if all_points:
        # 第一帧
        if all_frames:
            f0 = all_frames[0]
            pts0 = all_points[f0['offset']:f0['offset'] + f0['num_points']]
            if pts0:
                visualize_pointcloud(pts0, f"Frame {f0['frame_id']}",
                                     os.path.join(pc_dir, f"frame_{f0['frame_id']:04d}.png"))
        # 最后一帧
        if len(all_frames) > 1:
            fl = all_frames[-1]
            ptsl = all_points[fl['offset']:fl['offset'] + fl['num_points']]
            if ptsl:
                visualize_pointcloud(ptsl, f"Frame {fl['frame_id']}",
                                     os.path.join(pc_dir, f"frame_{fl['frame_id']:04d}.png"))
        # 全部
        visualize_pointcloud(all_points, "All Frames",
                             os.path.join(pc_dir, "all_frames.png"))

    print(f"\n输出目录: {output_base}")
    print("  pointclouds/ - 点云数据及图像")
    print("  rdmaps/      - Range-Doppler Map")
    print("  stats/        - 检测统计")
    return all_frames, all_points


if __name__ == "__main__":
    frames, points = main()
