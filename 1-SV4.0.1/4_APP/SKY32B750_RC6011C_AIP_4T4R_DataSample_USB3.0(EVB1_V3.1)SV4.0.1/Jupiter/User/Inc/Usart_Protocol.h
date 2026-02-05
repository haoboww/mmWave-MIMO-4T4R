#ifndef __USART_PROTOCOL_H
#define __USART_PROTOCOL_H
/*Driver*/

#include "bsp_UART.h"
#include "hal_WDT.h"
/*User*/
//#include "BB_Config.h"
//#include "RF_SPI_Config.h"
#include "ParamConfig.h"

/*
    num of bit
    NUM_BIT4: 0xF,             have  4 bits
    NUM_BIT8: 0xFF,            have  8 bits
    NUM_BIT16: 0xFFFF,         have  16 bits
*/
#define NUM_BIT4                  (4)
#define NUM_BIT32                  (32)
/*
    num of BYTE
    NUM_BYTE16: 0xFFFF,                     have  2 bytes
    NUM_BYTE32: 0xFFFFFFFF,                 have  4 bytes
    NUM_BYTE64: 0xFFFFFFFFFFFFFFFF,         have  8 bytes
*/
#define NUM_BYTE8                   (1)
#define NUM_BYTE16                  (2)
#define NUM_BYTE32                  (4)
#define NUM_BYTE64                  (8)
/*
    num of hex
    NUM_HEX16: 0xFFFF,             have  4 hex characters
    NUM_HEX32: 0xFFFFFFFF,         have  8 hex characters
    NUM_HEX64: 0xFFFFFFFFFFFFFFFF, have  16 hex characters
*/
#define NUM_HEX4                  (1)
#define NUM_HEX8                  (2)
#define NUM_HEX16                 (4)
#define NUM_HEX32                 (8)
#define NUM_HEX64                 (16)

//AT
#define AT_LEN                      (2)
#define AT_START_LEN                (8)
#define AT_RESET_LEN                (8)
#define AT_IMPORT_LEN       (9)

#define AT_GAP_MIN        (80)    //80us
#define AT_GAP_MAX 				(3200)  //3.2ms
#define AT_FGAP_MIN       (1)     //Ã¿1Ö¡´æ
#define AT_FGAP_MAX       (99)    //Ã¿99Ö¡´æ
#define AT_MAX_TIME         (60000)
#define AT_MAX_PROG         (2)
#define AT_MIN_PROG         (1)
#define AT_MAX_CFAR       (99)
#define AT_MAX_DPK        (99)
#define AT_MAX_DPKTIMES        (16)
#define AT_MAX_NUMTLV     (3)
#define AT_MIN_NUMTLV     (3)
#define AT_MAX_ZO       (99)














#define AT_PASET_LEN           (9)
#define AT_PASET_TOTAL_LEN     (AT_PASET_LEN+NUM_HEX8)
#define AT_FUNC_LEN            (8)
#define AT_FUNC_TOTAL_LEN     (AT_FUNC_LEN+NUM_HEX16)

#define MAX_BBR8_LEN (1048576)
#define MAX_BBR16_LEN (1048576/2)
#define MAX_BBR32_LEN (1048576/4)
#define MAX_BBR64_LEN (1048576/8)
void RecProtocol_Loop(void);


typedef struct
{
    uint8_t SN;
    uint8_t row ;
    uint8_t col ;
    uint8_t addr8 ;
    uint32_t addr32 ;
    uint32_t len;
    uint8_t data8 ;
    uint16_t data16 ;
    uint32_t data32 ;
    uint64_t data64 ;
}STRUCT_UDATA;
void ImportData_Loop(void);
uint32_t ProtocolSendPack(uint32_t targetNum,STRUCT_FRAMEPOINT *sPtRst,uint8_t *sBuff,uint16_t sTime);

#endif

