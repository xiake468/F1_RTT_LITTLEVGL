#ifndef _CONTROL_DATA_H__
#define  _CONTROL_DATA_H__
 
#include <rthw.h>
#include <rtthread.h>
#include "gd32f20x.h"
#include "main.h"

typedef struct
{
    uint8_t u8Power;
    uint8_t  u8FanMode;
    uint8_t   u8FanLevel;
    uint16_t u16LcdBright;
}ControlData;

void vLcdBrightSetter(uint16_t u16TempBright);
uint16_t u16LcdBrightGetter(void);
void vPowerSetter(uint8_t u8TempPower);
uint8_t u8PowerGetter(void);
void vFanModeSetter(uint8_t u8TempFanMode);
uint8_t u8FanModeGetter(void);
void vFanLevelSetter(uint8_t u8TempFanLevel);
uint8_t u8FanLevelGetter(void);

#endif



