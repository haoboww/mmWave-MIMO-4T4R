/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _APP_DEF_H
#define _APP_DEF_H

#include <stdint.h>
#include "SKY32B750.h"
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
#define NUM_BYTE192                 (24)

typedef struct{
	float fltReal;
	float fltImag;
}STRUCT_COMPLEX_FLT;
typedef struct{
	int intReal;
	int intImag;
}STRUCT_COMPLEX_INT;

typedef struct{
	uint16_t d1Idx;
//	uint16_t d1IdxFine;
	uint16_t d2Idx;
//	uint16_t d2IdxFine;
	int16_t  d2IdxRslt;
//	int32_t  d2IdxFineRslt;
	uint16_t d3Idx;
//	uint16_t d3IdxFine;
	uint16_t d4Idx;
	uint16_t cfarIdx;
	uint32_t pow2dAbs;
	uint32_t pow3dAbs;
	uint32_t noise; 
	uint8_t snr;
}STRUCT_FRAMEPOINT;

#define RF_RC7711C
//#define RF_RC7701AM

#endif

