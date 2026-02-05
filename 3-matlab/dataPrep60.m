clear all;
commonCfg.numByte       = 2;
commonCfg.numSamp       = 512;
commonCfg.fig           = 1;
commonCfg.numChirp      = 64;
commonCfg.numANT        = 16;
commonCfg.frameCount    = 1;
Head = 0;
dataFac=3000;
len = commonCfg.numSamp*commonCfg.numChirp*commonCfg.numANT*commonCfg.numByte;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
dataBlock=commonCfg.numByte * commonCfg.numSamp;
frameStart  =1;
% frameEnd    =2105;
frameEnd    =30;
if (0) %生成校准值
    load('amp_calib.mat');
    load('phase_calib.mat');
    load('freq_calib.mat');
    calib_over_buf = single(ones(commonCfg.numANT,commonCfg.numSamp));
    calib_ap = amp_calib .* exp(1j * phase_calib);  % 振幅和相位
    calib_nco = exp(1j * 2 * pi * freq_calib * [0:commonCfg.numSamp-1] / commonCfg.numSamp); %频率
    calib_over_buf=calib_over_buf.*calib_nco;
    for k=1:commonCfg.numSamp
        calib_over_buf(:,k)=calib_over_buf(:,k).*calib_ap;
    end
    calib_over_buf=calib_over_buf.';
    calib_over_buf  = repmat(reshape(calib_over_buf, [commonCfg.numSamp, 1, commonCfg.numANT]),1, commonCfg.numChirp, 1);
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
frameCurNum=1;
for frameNow=frameStart:frameEnd
    fid = fopen(['D:\Codes\4.0.1\2-exe\exe\exe\data\flash_data_' num2str(frameNow-1) '.bin'], 'rb');
    if(fid==-1)
        break;
    end
    [FlashReadBuf] = fread(fid);
    fclose(fid);
    for i=1:len/8
        FrameDataBuff((i-1)*4+1) = bitshift(FlashReadBuf((i-1)*8+7),8)+FlashReadBuf((i-1)*8+8);
        FrameDataBuff((i-1)*4+2) = bitshift(FlashReadBuf((i-1)*8+5),8)+FlashReadBuf((i-1)*8+6);
        FrameDataBuff((i-1)*4+3) = bitshift(FlashReadBuf((i-1)*8+3),8)+FlashReadBuf((i-1)*8+4);
        FrameDataBuff((i-1)*4+4) = bitshift(FlashReadBuf((i-1)*8+1),8)+FlashReadBuf((i-1)*8+2);
    end 
    for i=1:length(FrameDataBuff)
        if(FrameDataBuff(i)>32767)
         FrameDataBuff(i) = FrameDataBuff(i)-65536;
        end
    end  

    for k=1:commonCfg.numChirp
        for j=1:commonCfg.numANT
            ADCBuf(:,k,j)=FrameDataBuff(1+commonCfg.numSamp*(j-1)+commonCfg.numSamp*commonCfg.numANT*(k-1):commonCfg.numSamp*j+commonCfg.numSamp*commonCfg.numANT*(k-1)); 
        end        
    end 

    gBuf(:,:,:,frameNow) = ADCBuf;
    if(1)
        figure(1);
        plot(squeeze(gBuf(:,:,1,frameNow)));
        hold on;
        for j=2:commonCfg.numANT
            plot(squeeze(gBuf(:,:,j,frameNow))+dataFac*(j-1));
        end
       % plot(abs(fft(ADCBuf(:,1,1))));
        %%
%         hold on;
%         dataFac = 3000;
%         for j=1:commonCfg.numANT
%             plot(squeeze(ADCBuf(:,:,j))+dataFac*(j-1));
%         end
        %
        hold off;
        ylim([-dataFac +dataFac*commonCfg.numANT]);
        title(frameNow);
    end
    disp(frameNow);
    pause(0.1);
end
save('D:\Codes\4.0.1\2-exe\exe\exe\data\gBuf.mat', 'gBuf');
disp('over');
