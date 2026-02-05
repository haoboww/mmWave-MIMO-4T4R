#include "hal_RF.h"
#include "hal_Delay.h"
#include "bsp_UART.h"
uint8_t SPI_write_data[10] = {0};
uint8_t SPI_read_data[10] = {0};
uint16_t RF_ReadRegSPI(uint8_t csNum, uint8_t u8Addr)
{
    uint16_t u16Data;
    SPI_write_data[0] = (u8Addr & 0x3F) | 0x40; // sky6605 SPI1 write
    SPI_write_data[1] = 0xFF;                   // send dumy data
    SPI_write_data[2] = 0xFF;                   // send dumy data                                        //à-μí
		__disable_irq();	
    RF_SPI_CS(csNum, 0);
    USCI_SPI_TRx_Ext(&SPI0, SPI_write_data, 3, SPI_read_data, 2);
    RF_SPI_CS(csNum, 1); // à-??
		__enable_irq();	
    SPI0_CS1_WritePin = 1;
    u16Data = (SPI_read_data[1] << 8) | SPI_read_data[2];
    //    printf("addr=%02x,data=%04x\n",u8Addr,u16Data);
    return u16Data;
}
void RF_WriteRegSPI(uint8_t csNum, uint8_t u8Addr, uint16_t u16Data)
{
		__disable_irq();
		if (RF_ReadRegSPI(csNum, u8Addr) == u16Data){}
		else
		{
			SPI_write_data[0] = (u8Addr & 0x3F);
			SPI_write_data[1] = u16Data >> 8;
			SPI_write_data[2] = u16Data;			
			RF_SPI_CS(csNum, 0);
			USCI_SPI_Transmit_Ext(&SPI0, SPI_write_data, 3, 2);
			RF_SPI_CS(csNum, 1);
			uint16_t rtn = RF_ReadRegSPI(csNum, u8Addr);
			if (rtn != u16Data)
			{
					printf("ERROR:cs=%d,addr=%02X,readData=%04X,oriData=%04X\n", csNum, u8Addr, rtn, u16Data);
			}
		}
		__enable_irq();
}
void RF_WriteRegSPI_Fast(uint8_t csNum, uint8_t u8Addr, uint16_t u16Data)
{
		
    SPI_write_data[0] = (u8Addr & 0x3F);
    SPI_write_data[1] = u16Data >> 8;
    SPI_write_data[2] = u16Data;
		__disable_irq();
    RF_SPI_CS(csNum, 0);
    USCI_SPI_Transmit_Ext(&SPI0, SPI_write_data, 3, 2);
    RF_SPI_CS(csNum, 1);
		__enable_irq();
}
void RF_WritePaSPI_AM(uint8_t csNum, uint8_t u8Addr, uint16_t u16Data)
{
    SPI_write_data[0] = (u8Addr & 0x3F);
    SPI_write_data[1] = u16Data >> 8;
    SPI_write_data[2] = u16Data;
		__disable_irq();
    RF_SPI_CS(csNum, 0);
    USCI_SPI_Transmit_Ext(&SPI0, SPI_write_data, 3, 2);
    RF_SPI_CS(csNum, 1);
		__enable_irq();
}
uint16_t RF_ReadWaveSPI_AM(uint8_t csNum, uint8_t Row_Addr, uint8_t Col_Addr)
{
    uint16_t u16Data;
    SPI_write_data[0] = 0x7E;
    SPI_write_data[1] = (Row_Addr & 0x1F) << 3 | (Col_Addr & 0x07);
    SPI_write_data[2] = 0xFF;
    SPI_write_data[3] = 0xFF;
    RF_SPI_CS(csNum, 0);
    USCI_SPI_TRx_Ext(&SPI0, SPI_write_data, 4, SPI_read_data, 2);
    RF_SPI_CS(csNum, 1);
    u16Data = ((uint16_t)SPI_read_data[2] << 8) | SPI_read_data[3];
    printf("seq=%d,row=%d,col=%d,data=%04X\n", csNum, Row_Addr, Col_Addr, u16Data);
    return u16Data;
}
void RF_WriteWaveSPI_AM(uint8_t csNum, uint8_t Row_Addr, uint8_t Col_Addr, uint16_t u16Data)
{
    SPI_write_data[0] = 0x3E;
    SPI_write_data[1] = (Row_Addr & 0x1F) << 3 | (Col_Addr & 0x07);
    SPI_write_data[2] = u16Data >> 8;
    SPI_write_data[3] = u16Data;
		__disable_irq();
    RF_SPI_CS(csNum, 0);
    USCI_SPI_Transmit_Ext(&SPI0, SPI_write_data, 4, 2);
    RF_SPI_CS(csNum, 1);
		__enable_irq();	
//    uint16_t rtn = RF_ReadWaveSPI_AM(csNum, Row_Addr, Col_Addr);
//    if (rtn != u16Data)
//    {
//    	printf("ERROR:Raddr=%02x,Caddr=%02x,Writedata=%04x,Readdata=%04X\n", Row_Addr, Col_Addr, u16Data, rtn);
//    		//while(1);
//    }
}

uint16_t RF_ReadWaveSPI_C(uint8_t csNum, uint8_t Row_Addr, uint8_t Col_Addr)
{
    uint16_t u16Data;
    SPI_write_data[0] = 0x7E;
    SPI_write_data[1] = Row_Addr & 0x7F;
    SPI_write_data[2] = (Col_Addr << 5) & 0xE0;
    SPI_write_data[3] = 0xFF;
    SPI_write_data[4] = 0xFF;
    RF_SPI_CS(csNum, 0);
    USCI_SPI_TRx_Ext(&SPI0, SPI_write_data, 5, SPI_read_data, 2);
    RF_SPI_CS(csNum, 1);
    u16Data = (SPI_read_data[3] << 8) | SPI_read_data[4];
    //		printf("Raddr=%02x,Caddr=%02x,data=%04x\n",Row_Addr,Col_Addr,u16Data);
    return u16Data;
}

void RF_WriteWaveSPI_C(uint8_t csNum, uint8_t Row_Addr, uint8_t Col_Addr, uint16_t u16Data)
{
    SPI_write_data[0] = 0x3E;
    SPI_write_data[1] = Row_Addr & 0x7F;
    SPI_write_data[2] = (Col_Addr << 5) & 0xE0;
    SPI_write_data[3] = (u16Data >> 8) & 0xFF;
    SPI_write_data[4] = u16Data & 0xFF;
    RF_SPI_CS(csNum, 0);
    USCI_SPI_Transmit_Ext(&SPI0, SPI_write_data, 5, 2);
    RF_SPI_CS(csNum, 1);
	
//		for(uint8_t i=0; i<2; i++){
//			uint16_t rtn = RF_ReadWaveSPI_C(csNum, Row_Addr, Col_Addr);
//			if (rtn != u16Data){
//				printf("Raddr=%02x,Caddr=%02x,Writedata=%04x,Readdata=%04X\n", Row_Addr, Col_Addr, u16Data, rtn);
//				//while(1);
//			}
//			else{
//				break;
//			}
//		}
		uint16_t rtn = RF_ReadWaveSPI_C(csNum, Row_Addr, Col_Addr);
		if (rtn != u16Data){
			printf("retry");
			RF_SPI_CS(csNum, 0);
			USCI_SPI_Transmit_Ext(&SPI0, SPI_write_data, 5, 2);
			RF_SPI_CS(csNum, 1);
			rtn = RF_ReadWaveSPI_C(csNum, Row_Addr, Col_Addr);
			if (rtn != u16Data){
				printf("Raddr=%02x,Caddr=%02x,Writedata=%04x,Readdata=%04X\n", Row_Addr, Col_Addr, u16Data, rtn);
				//while(1);
			}
		}
}
void RF_WaveSet(uint8_t Row_Addr, RF_WAVE_CFG sWave)
{
#ifdef RF_RC7701AM
    RF_WriteWaveSPI_AM(CS1_PIN, Row_Addr, 0x07, sWave.a1);
    RF_WriteWaveSPI_AM(CS1_PIN, Row_Addr, 0x06, sWave.a2);
    RF_WriteWaveSPI_AM(CS1_PIN, Row_Addr, 0x05, sWave.b1);
    RF_WriteWaveSPI_AM(CS1_PIN, Row_Addr, 0x04, sWave.b2);
    RF_WriteWaveSPI_AM(CS1_PIN, Row_Addr, 0x03, sWave.c1);
    RF_WriteWaveSPI_AM(CS1_PIN, Row_Addr, 0x02, sWave.c2);
    RF_WriteWaveSPI_AM(CS1_PIN, Row_Addr, 0x01, sWave.r);
    RF_WriteWaveSPI_AM(CS1_PIN, Row_Addr, 0x00, sWave.v);
#endif
#ifdef RF_RC7711C
    RF_WriteWaveSPI_C(CS1_PIN, Row_Addr, 0x07, sWave.a1);
    RF_WriteWaveSPI_C(CS1_PIN, Row_Addr, 0x06, sWave.a2);
    RF_WriteWaveSPI_C(CS1_PIN, Row_Addr, 0x05, sWave.b1);
    RF_WriteWaveSPI_C(CS1_PIN, Row_Addr, 0x04, sWave.b2);
    RF_WriteWaveSPI_C(CS1_PIN, Row_Addr, 0x03, sWave.c1);
    RF_WriteWaveSPI_C(CS1_PIN, Row_Addr, 0x02, sWave.c2);
    RF_WriteWaveSPI_C(CS1_PIN, Row_Addr, 0x01, sWave.r);
    RF_WriteWaveSPI_C(CS1_PIN, Row_Addr, 0x00, sWave.v);
#endif
}
void RF_WriteCmdSPI(uint8_t u8Cmd)
{
    u8Cmd = (u8Cmd & 0x3F) | 0x80;
		__disable_irq();
    SPI0_CS1_WritePin = 0;
    USCI_SPI_Transmit_Ext(&SPI0, &u8Cmd, 1, 2);
    SPI0_CS1_WritePin = 1;
		__enable_irq();
}
void RF_Clock40M_En(void)
{
    BSP_ChipEn();
#if 0
		while(1){
			uint16_t rtn=RF_ReadRegSPI(CS1_PIN,0);
			printf("rtn=%04X\n",rtn);
			Cnt_Delay_ms(500);
		}
#endif
#ifdef RF_RC7701AM
    RF_WriteRegSPI(CS1_PIN, ANA_CFG21_ADDR, 0x0001);
#endif
#ifdef RF_RC7711C
		RF_WriteRegSPI(CS1_PIN, ANA_CFG2_ADDR, 0x0001);
#endif
}
void RF_waveStart(void) /*{{{*/
{
    RF_WriteCmdSPI(0x00); // waveform generator start
} /*}}}*/
void RF_waveStop(void) /*{{{*/
{
    RF_WriteCmdSPI(0x02); // waveform generator start
} /*}}}*/
/******************************************calc freq ***********************************************/
int mod(int a, int b)
{
    return a % b;
}
/*初始频率计算函数
 * freqstart：输入的初始频率，单位为GHz，比如77代表77GHz
 * rtn：返回值为计算结果
 */
uint32_t RF_Freq_Start_Calc(float freqstart)
{
    uint32_t rtn = 0;
    // 整数部分定义
    float G7 = 0.0;
    int H7 = 0;
    int value = 0;
    // 小数部分定义
    float I7 = 0.0;
    float ftemp[42] = {0.0};
    int itemp[21] = {0};
    // printf("freq=%f\n", freqstart);
    G7 = freqstart * 1000 / 16 / 40; // 40为RF芯片晶振频率
    H7 = (int)G7 - 64;               // 整数部分
    I7 = G7 - (int)G7;
    // 整数部分计算
    // Bit11+16
    value = (int)H7 / 2;
    for (uint32_t i = 0; i < 5; i++)
    {
        value = (int)value / 2;
    }
    rtn |= mod(value, 2) << (11 + 16);
    // Bit10+16
    value = (int)H7 / 2;
    for (uint32_t i = 0; i < 4; i++)
    {
        value = (int)value / 2;
    }
    rtn |= mod(value, 2) << (10 + 16);
    // Bit9+16
    value = (int)H7 / 2;
    for (uint32_t i = 0; i < 3; i++)
    {
        value = (int)value / 2;
    }
    rtn |= mod(value, 2) << (9 + 16);
    // Bit8+16
    value = (int)H7 / 2;
    for (uint32_t i = 0; i < 2; i++)
    {
        value = (int)value / 2;
    }
    rtn |= mod(value, 2) << (8 + 16);
    // Bit7+16
    value = (int)H7 / 2;
    value = (int)value / 2;
    rtn |= mod(value, 2) << (7 + 16);
    // Bit6+16
    value = (int)H7 / 2;
    rtn |= mod(value, 2) << (6 + 16);
    // Bit5+16
    value = (int)H7 / 2;
    rtn |= mod(H7, 2) << (5 + 16);
    // 小数部分计算
    // Bit4+16
    ftemp[0] = I7 * 2;              // I37
    itemp[0] = (int)ftemp[0];       // I42
    ftemp[1] = ftemp[0] - itemp[0]; // I38
    rtn |= itemp[0] << (4 + 16);
    // Bit[13+16:0]
    for (uint32_t i = 0; i < 20; i++)
    {
        ftemp[2 * i + 2] = ftemp[2 * i + 1] * 2;            // AB37
        itemp[i + 1] = (int)ftemp[2 * i + 2];               // AB42
        ftemp[2 * i + 3] = ftemp[2 * i + 2] - itemp[i + 1]; // AB38
        rtn |= itemp[i + 1] << (16 + 3 - i);
    }
    return rtn;
}
/*扫频步进控制字计算函数，ABC段需要分别调用，不包含Bit[1:0]RAMSYNC的设置
 * BW：扫频带宽，单位GHz
 * Sweep_time：扫频时间，单位us
 * PM：扫频方向，0为正，1为负
 * RefClk: 参考时钟，单位MHz
 * rtn：返回值为计算结果
 */
static uint32_t RF_Sweep_step_Calculate(float BW, float Sweep_time, uint8_t PM, float RefClk)
{
    uint32_t rtn = 0;
    float Freq_step = 0.0; // 步进频率
    float G5 = 0.0;        // 8Tref量化的扫描时长
    int E21 = 0;           // 扫描时长量化值
    float E27 = 0.0;       // 分频比量化值（5g_pll）
    float G8 = 0.0;        // 参考时钟周期，40为晶振频率40MHz，G8单位是us
    float ftemp[30] = {0.0};
    int itemp[32] = {0};

    G8 = 1.0f / RefClk;
    G5 = ((Sweep_time / (8 * G8))) * 8 * G8;
    E21 = (int)(G5 / (8 * G8));
    Freq_step = BW * 1000.0f * 1000.0f / (E21 * 8.0f);
    E27 = Freq_step / 16 / 40000;

    // Bit 15+16
    rtn |= (uint32_t)((PM & 0x01) << (15 + 16)); // 扫频方向，0为正，1为负
    // Bit 14+16
    ftemp[0] = E27 * 32;            // E30
    itemp[0] = (int)(ftemp[0]);     // E35
    ftemp[1] = ftemp[0] - itemp[0]; // E31
    rtn |= (uint32_t)(itemp[0] << (14 + 16));
    // Bit[13+16:0+16]
    for (uint32_t i = 0; i < 14; i++)
    {
        ftemp[2 * i + 2] = ftemp[2 * i + 1] * 2;            // F30-S30
        itemp[i + 1] = (int)(ftemp[2 * i + 2]);             // F35-S35
        ftemp[2 * i + 3] = ftemp[2 * i + 2] - itemp[i + 1]; // F31-S31
        rtn |= (uint32_t)(itemp[i + 1] << (13 + 16 - i));
    }
    // Bit [15:3]
    itemp[16] = (int)(E21 / 2); // D23
    for (uint32_t i = 0; i < 12; i++)
    {
        itemp[i + 17] = (int)(itemp[i + 16] / 2); // E23-P23
    }
    for (uint32_t i = 0; i < 13; i++)
    {
        rtn |= (uint32_t)(mod(itemp[16 + i], 2) << (3 + i)); // D23-P23
    }
    // Bit 2
    rtn |= (uint32_t)(mod(E21, 2) << 2); // E21
    return rtn;
}

RF_WAVE_CFG Hal_RF_WaveCalc(float bandwidthA, float bandwidthB, float bandwidthC, float sweepTimA, float sweepTimB, float sweepTimC)
{
    RF_WAVE_CFG rtn;
    uint32_t regVal;
    regVal = RF_Sweep_step_Calculate(bandwidthA, sweepTimA, 1, 40) + 1;
    rtn.a1 = regVal >> 16;
    rtn.a2 = regVal & 0xFFFF;
    regVal = RF_Sweep_step_Calculate(bandwidthB, sweepTimB, 1, 40);
    rtn.b1 = regVal >> 16;
    rtn.b2 = regVal & 0xFFFF;
    regVal = RF_Sweep_step_Calculate(bandwidthC, sweepTimC, 0, 40);
    rtn.c1 = (regVal >> 16) | 0x8000;
    rtn.c2 = regVal & 0xFFFF;
    //	printf("a1=%04X,a2=%04X,b1=%04X,b2=%04X,c1=%04X,c2=%04X\n",rtn.a1,rtn.a2,rtn.b1,rtn.b2,rtn.c1,rtn.c2);
    return rtn;
}

RF_WAVE_CFG Hal_RF_WaveGen(float bandwidthA, float sweepTimA, uint8_t syncwA, float bandwidthB, float sweepTimB, uint8_t syncwB, float bandwidthC, float sweepTimC, uint8_t syncwC, uint16_t r, uint16_t v)
{
    RF_WAVE_CFG rtn;
    uint32_t regVal;
	  regVal = bandwidthA >= 0 ? (RF_Sweep_step_Calculate(bandwidthA, sweepTimA, 0, 40) | syncwA) : (RF_Sweep_step_Calculate(-bandwidthA, sweepTimA, 1, 40) | syncwA);
    rtn.a1 = regVal >> 16;
    rtn.a2 = regVal & 0xFFFF;
	  regVal = bandwidthB >= 0 ? (RF_Sweep_step_Calculate(bandwidthB, sweepTimB, 0, 40) | syncwB) : (RF_Sweep_step_Calculate(-bandwidthB, sweepTimB, 1, 40) | syncwB); 
    rtn.b1 = regVal >> 16;
    rtn.b2 = regVal & 0xFFFF;
		regVal = bandwidthC >= 0 ? (RF_Sweep_step_Calculate(bandwidthC, sweepTimC, 0, 40) | syncwC) : (RF_Sweep_step_Calculate(-bandwidthC, sweepTimC, 1, 40) | syncwC); 
    rtn.c1 = regVal >> 16;
    rtn.c2 = regVal & 0xFFFF;
	  rtn.r = r;
	  rtn.v = v;
//    printf("a1=%04X,a2=%04X,b1=%04X,b2=%04X,c1=%04X,c2=%04X,r=%04X,v=%04X\n",rtn.a1,rtn.a2,rtn.b1,rtn.b2,rtn.c1,rtn.c2,rtn.r,rtn.v);
    return rtn;
}

RF_WAVE_CFG Hal_RF_WaveConfig(uint16_t a1, uint16_t a2, uint16_t b1, uint16_t b2, uint16_t c1, uint16_t c2, uint16_t r, uint16_t v)
{
    RF_WAVE_CFG rtn;
    rtn.a1 = a1;
    rtn.a2 = a2;
    rtn.b1 = b1;
    rtn.b2 = b2;
    rtn.c1 = c1;
    rtn.c2 = c2;
    rtn.r = r;
    rtn.v = v;
    return rtn;
}
