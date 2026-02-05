%% radar_calib_process_realADC.m
% 说明：
% - 你的ADC是实采样（无虚部），校准因子是复数（幅相/频偏补偿），因此会把ADC数据转为 complex(x,0)
% - 保存采用 matfile 逐帧写盘，避免内存爆
% - 绘图默认画 real 部分

clear; clc;

%% ====================== 基本配置 ======================
commonCfg.numByte   = 2;      % int16
commonCfg.numSamp   = 512;
commonCfg.numChirp  = 64;
commonCfg.numANT    = 16;

frameStart  = 1;
frameEnd    = 30;           % 你的最大帧数
% frameEnd    = 2105;           % 你的最大帧数
dataFac     = 3000;           % 画图用的offset
doPlot      = true;           % 是否每帧画图
pauseSec    = 0.05;           % 每帧暂停（画图刷新用）

useCalib    = true;           % <<< 打开/关闭校准

% 数据路径与文件名
baseDir = 'D:\Codes\通道补偿代码\radar_calib\';
filePrefix = 'flash_data_';
fileSuffix = '.bin';

% 每帧样点数（int16个数）
numSamplesPerFrame = commonCfg.numSamp * commonCfg.numChirp * commonCfg.numANT;
lenBytesPerFrame   = numSamplesPerFrame * commonCfg.numByte;

%% ====================== 生成/加载校准因子 ======================
% 目标：calib_over_buf 的尺寸 = [numSamp, numChirp, numANT]，复数 single
% 其中每个天线在每个sample点上有一个复数补偿（幅相 + 频偏NCO），并对所有chirp重复

calib_over_buf = [];

if useCalib
    % 你已有的校准文件
    load(fullfile(baseDir, 'amp_calib.mat'));    % 期望变量：amp_calib
    load(fullfile(baseDir, 'phase_calib.mat'));  % 期望变量：phase_calib
    load(fullfile(baseDir, 'freq_calib.mat'));   % 期望变量：freq_calib

    % --- 统一amp/phase维度到 [numANT, numSamp] 或 [numANT,1] 兼容 ---
    % 常见情况：
    %   amp_calib:   [numANT,1] 或 [numANT,numSamp]
    %   phase_calib: [numANT,1] 或 [numANT,numSamp]
    %   freq_calib:  [numANT,1] 或 [numANT,numSamp]（少见）

    % 转 single
    amp_calib   = single(amp_calib);
    phase_calib = single(phase_calib);
    freq_calib  = single(freq_calib);

    % 幅相合成：calib_ap，尺寸尽量变成 [numANT, numSamp]
    calib_ap = amp_calib .* exp(1j * phase_calib);

    if size(calib_ap,2) == 1
        % [numANT,1] -> 扩到 [numANT,numSamp]
        calib_ap = repmat(calib_ap, 1, commonCfg.numSamp);
    elseif size(calib_ap,2) ~= commonCfg.numSamp
        error('calib_ap 维度不匹配：期望 [numANT,1] 或 [numANT,numSamp]，实际是 [%d,%d]', size(calib_ap,1), size(calib_ap,2));
    end

    % 频偏NCO：calib_nco，尺寸 [numANT, numSamp]
    n = single(0:commonCfg.numSamp-1); % 1×numSamp

    if isvector(freq_calib)
        freq_calib = freq_calib(:); % [numANT,1]
        if numel(freq_calib) ~= commonCfg.numANT
            error('freq_calib 向量长度不等于 numANT：%d vs %d', numel(freq_calib), commonCfg.numANT);
        end
        calib_nco = exp(1j * 2*pi * (freq_calib * n) / commonCfg.numSamp); % [numANT,numSamp]
    else
        % freq_calib 若给了 [numANT,numSamp]，则逐点乘 n（广播）
        if size(freq_calib,1) ~= commonCfg.numANT || size(freq_calib,2) ~= commonCfg.numSamp
            error('freq_calib 维度不匹配：期望 [numANT,1] 或 [numANT,numSamp]，实际是 [%d,%d]', size(freq_calib,1), size(freq_calib,2));
        end
        calib_nco = exp(1j * 2*pi * (freq_calib .* repmat(n, commonCfg.numANT, 1)) / commonCfg.numSamp);
    end

    % 总补偿：每个天线每个sample一个复数系数
    calib_ANTxSamp = calib_ap .* calib_nco;                 % [numANT,numSamp]

    % 变形为 [numSamp, 1, numANT] 再复制到 chirp 维
    calib_samp_1_ant = permute(calib_ANTxSamp, [2,3,1]);    % [numSamp,1,numANT]
    calib_over_buf   = repmat(calib_samp_1_ant, 1, commonCfg.numChirp, 1); % [numSamp,numChirp,numANT]

    calib_over_buf = single(calib_over_buf); % 复数single
else
    % 不校准时用全1（仍然定义出来，方便代码统一）
    calib_over_buf = single(ones(commonCfg.numSamp, commonCfg.numChirp, commonCfg.numANT));
end

%% ====================== 输出 matfile（逐帧写盘） ======================
outMat = fullfile(baseDir, 'gBuf_calib.mat');
m = matfile(outMat, 'Writable', true);

% 预先在文件里“声明”一个最终大小（不会一次性占满RAM）
% 注意：matfile 会在磁盘上按需写入，但第一次赋值时会创建变量结构
m.gBuf = complex(zeros(commonCfg.numSamp, commonCfg.numChirp, commonCfg.numANT, frameEnd, 'single'), 0);

%% ====================== 主循环：逐帧读取、组帧、校准、画图、写盘 ======================
for frameNow = frameStart:frameEnd

    fname = fullfile(baseDir, [filePrefix num2str(frameNow-1) fileSuffix]);
    fid = fopen(fname, 'rb');
    if fid == -1
        fprintf('[Stop] 文件不存在或无法打开：%s\n', fname);
        break;
    end

    % 读原始bytes（uint8）
    rawU8 = fread(fid, inf, '*uint8');
    fclose(fid);

    if numel(rawU8) < lenBytesPerFrame
        fprintf('[Stop] 帧 %d 数据长度不足：读到 %d bytes，期望至少 %d bytes\n', frameNow, numel(rawU8), lenBytesPerFrame);
        break;
    end

    % 只取当前帧需要的长度（避免尾部多余）
    rawU8 = rawU8(1:lenBytesPerFrame);

    % ================== 你的原始“8字节->4个16bit”重排逻辑（向量化版） ==================
    % 原代码：
    %   每 8 bytes => 4 个 int16，顺序为 (7,8),(5,6),(3,4),(1,2)
    % 这里严格复现

    u = reshape(rawU8, 8, []);  % 8 × (len/8)
    w1 = uint16(u(7,:)) * 256 + uint16(u(8,:));
    w2 = uint16(u(5,:)) * 256 + uint16(u(6,:));
    w3 = uint16(u(3,:)) * 256 + uint16(u(4,:));
    w4 = uint16(u(1,:)) * 256 + uint16(u(2,:));

    wordsU16 = [w1; w2; w3; w4];          % 4 × (len/8)
    wordsU16 = wordsU16(:);               % (len/2) × 1

    % 转成 int16（有符号）
    frameI16 = typecast(wordsU16, 'int16');  % int16 vector, length = numSamplesPerFrame

    if numel(frameI16) ~= numSamplesPerFrame
        fprintf('[Stop] 帧 %d 样点数异常：%d vs 期望 %d\n', frameNow, numel(frameI16), numSamplesPerFrame);
        break;
    end

    % ================== reshape 成 ADCBuf: [numSamp, numChirp, numANT] ==================
    % 你的原循环索引等价于：
    %   先按 (sample) 连续
    %   再按 (ant) 递增
    %   再按 (chirp) 递增
    %
    % 所以先 reshape 为 [numSamp, numANT, numChirp]，再 permute
    tmp = reshape(frameI16, commonCfg.numSamp, commonCfg.numANT, commonCfg.numChirp);
    ADCBuf_real = permute(tmp, [1,3,2]);  % [numSamp, numChirp, numANT]，实数 int16

    % ================== 应用校准（实采样 -> 先转复数再乘复数校准因子） ==================
    ADCBuf = complex(single(ADCBuf_real), 0); % 复数single，虚部为0
    if useCalib
        ADCBuf = ADCBuf .* calib_over_buf;    % 关键：逐点校准
    end

    % ================== 写入 matfile（逐帧写盘） ==================
    m.gBuf(:,:,:,frameNow) = ADCBuf;

    % ================== 可视化（画 real 部分） ==================
    if doPlot
        figure(1); clf;
        hold on;
        % 画第1根天线（不加offset）
        plot(real(ADCBuf(:,:,1)));
        % 其他天线加offset便于观察
        for j = 2:commonCfg.numANT
            plot(real(ADCBuf(:,:,j)) + dataFac*(j-1));
        end
        hold off;
        ylim([-dataFac, dataFac*commonCfg.numANT]);
        title(sprintf('frameNow = %d (useCalib=%d)', frameNow, useCalib));
        drawnow;
        pause(pauseSec);
    end

    disp(frameNow);
end

disp('over');
fprintf('保存完成：%s\n', outMat);
