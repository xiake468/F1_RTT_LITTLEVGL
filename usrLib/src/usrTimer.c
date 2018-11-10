#include "usrTimer.h"

#define _1US_CNT 10
/*-----------------------------------------------------------*/
void Delay(uint32_t u32Count)
{
    while(u32Count != 0)
    {
        u32Count--;
    }
}

void vDelay_us(uint32_t u32Count)
{
    while(u32Count != 0)
    {
        u32Count--;
        Delay(_1US_CNT);

    }
}
/*-----------------------------------------------------------*/
void vDelay_ms(uint16_t nms)
{
    uint16_t i;

    for(i = 0; i < nms; i++) vDelay_us(1000);
}
/*-----------------------------------------------------------*/




