/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RF_CONFIG_H
#define _RF_CONFIG_H


#include "hal_RF.h"
#include "Radar_Config.h"


void RF_CfgInit(RADAR_USER_CFG *usrCfg);
void RF_Reg_Set(void);
void RF_Reg_Reset(void);
void RF_Dataprep_Switch(uint16_t rampCnt);
#endif /*_RF_CONFIG_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























