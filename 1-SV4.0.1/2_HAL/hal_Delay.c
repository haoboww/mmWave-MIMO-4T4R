#include "hal_Delay.h"
#include "hal_SysClk.h"
#include "SKY32B750.h"


/**
 * @Breif  : SW_Delay_us : delay n mircoseconds use software
 *
 * @Param  : u32Cnt: delay time
 */
void Cnt_Delay_us(uint32_t u32Cnt)
{
    uint32_t i, j;
    for (j = 0; j < u32Cnt; j++)
    {

        for(i = 0; i < 100; i++); //160
    }
}

/**
 * @Breif  : SW_Delay_ms : delay n milliseconds use software
 *
 * @Param  : u32Cnt: delay time
 */
void Cnt_Delay_ms(uint32_t u32Cnt)
{
    uint32_t j;
    uint32_t usCnt=hclk/158000;
    for (j = 0; j < u32Cnt; j++)
    {
       Cnt_Delay_us(usCnt);
    }
}


