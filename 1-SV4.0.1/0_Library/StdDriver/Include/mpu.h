
#ifndef __MPU_H__
#define __MPU_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define MPU_HFNMI_PRIVDEF_NONE          (0x00000000ul)
#define MPU_HARDFAULT_NMI               (0x00000002ul)
#define MPU_PRIVILEGED_DEFAULT          (0x00000004ul)
#define MPU_HFNMI_PRIVDEF               (0x00000006ul)

typedef struct
{
    uint8_t  enable;
    uint8_t  number;
    uint32_t baseAddress;
    uint8_t  size;
    uint8_t  subRegionDisable;
    uint8_t  typeExtField;
    uint8_t  accessPermission;
    uint8_t  disableExec;
    uint8_t  isShareable;
    uint8_t  isCacheable;
    uint8_t  isBufferable;
} MPU_Region_InitTypeDef;

void MPU_Config(void);
void MPU_ConfigRegion(MPU_Region_InitTypeDef *MPU_Init);

#ifdef __cplusplus
}
#endif


#endif
