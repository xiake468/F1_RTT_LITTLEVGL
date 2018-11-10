#ifndef __USR_BUZZ_H__
#define __USR_BUZZ_H__

#include <rthw.h>
#include <rtthread.h>
#include "gd32f20x.h"
#include "main.h"

void vInitBuzz(void);
void vSetBuzzFreqAndLoud(uint16_t u16Freq, uint8_t u8LoudPersent);

#endif


