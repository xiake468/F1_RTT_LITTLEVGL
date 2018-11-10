#include "ControlData.h"
#include "sensorPowerCtrl.h"

static ControlData controlData;

void vLcdBrightSetter(uint16_t u16TempBright)
{
    if(u16TempBright >= 1000)
        u16TempBright = 1000;

    controlData.u16LcdBright = u16TempBright;
}

uint16_t u16LcdBrightGetter(void)
{
    return controlData.u16LcdBright;
}

void vPowerSetter(uint8_t u8TempPower)
{
    if(controlData.u8Power != u8TempPower)
    {
        controlData.u8Power = u8TempPower;
    }
}

uint8_t u8PowerGetter(void)
{
    return controlData.u8Power;
}

void vFanModeSetter(uint8_t u8TempFanMode)
{
    controlData.u8FanMode = u8TempFanMode;
}

uint8_t u8FanModeGetter(void)
{
    return controlData.u8FanMode;
}

void vFanLevelSetter(uint8_t eTempFanLevel)
{
    controlData.u8FanLevel = eTempFanLevel;
}

uint8_t u8FanLevelGetter(void)
{
    return controlData.u8FanLevel;
}

