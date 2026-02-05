#include "hal_Radar_Calib.h"


uint32_t flt2pflt(float real_part, float imag_part)
{
	uint32_t *real_addr_p = (uint32_t *)&real_part;
	uint32_t *imag_addr_p = (uint32_t *)&imag_part;
	// [2] 写入实部数据
	Write_M32(0x30800000, real_addr_p[0]);
	// [3] 写入虚部数据
	Write_M32(0x30800004, imag_addr_p[0]);
	// [4] 读取转换后的pseudo-floating数据
	uint32_t rtn = Read_M32(0x30000000);

	return rtn;
}


void HAL_CalibDataLoop(STRUCT_UART strUart, uint32_t num_calib_ch, uint32_t len)
{
	if (strUart.writeCnt >= len)
	{
		uint8_t writeFlashCnt = 0;
		strUart.writeCnt = 0;
		stFlashParam.WorkMode = 0;

		printf("flt2pf calibFreq Data:\n");
		for (int i = 0; i < algCfg.numTX * algCfg.numRX; ++i)
		{
			uint32_t *calibr_addr_u = (uint32_t *)(CALIBDATA_ADDR + i * NUM_BIT4);
			uint32_t rtn = *calibr_addr_u;
			printf("[%d]0x%08X\n", i, rtn);
			while (EFC_ReWrite(FLASH_CALIB_FREQ_ADDR + i * NUM_BIT4, NUM_BIT4, (uint8_t *)&rtn))
			{
				vTaskDelay(pdMS_TO_TICKS(50));
			}
			writeFlashCnt++;
		}

		BUF_CFG0(BUF_CFG0_WR_FLT2PF_LSF_EN_RIGHT, 0, BUF_CFG0_CONVERT_CPX_MODE_FLOAT, BUF_CFG0_RD_POWMODE_ABS);
		printf("flt2pf calibAp Data:\n");
		for (int i = 0; i < algCfg.numTX * algCfg.numRX; ++i)
		{
			float *real_part = (float *)(CALIBDATA_ADDR + (algCfg.numTX * algCfg.numRX + 2 * i) * NUM_BIT4);
			float *imag_part = (float *)(CALIBDATA_ADDR + (algCfg.numTX * algCfg.numRX + 2 * i + 1) * NUM_BIT4);
			uint32_t rtn = flt2pflt(real_part[0], imag_part[0]);
			printf("[%d]0x%08X\n", i, rtn);
			while (EFC_ReWrite(FLASH_CALIB_AP_ADDR + i * NUM_BIT4, NUM_BIT4, (uint8_t *)&rtn))
			{
				vTaskDelay(pdMS_TO_TICKS(50));
			}
			writeFlashCnt++;
		}

		if (writeFlashCnt == algCfg.numTX * algCfg.numRX * 2)
		{
			printf("AT+CALIBCOMPLETE\n");
			writeFlashCnt = 0;
		}
	}
}


