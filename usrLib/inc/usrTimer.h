#ifndef __USR_TIMER_H__
#define __USR_TIMER_H__

#include <rthw.h>
#include <rtthread.h>
#include "gd32f20x.h"
#include "main.h"

void vDelay_us(uint32_t u32Count);
void vDelay_ms(uint16_t nms);

#endif



