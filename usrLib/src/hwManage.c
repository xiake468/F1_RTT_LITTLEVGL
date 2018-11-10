#include "hwManage.h"
#include "driveLcd.h"
#include "lcdBright.h"
#include "ControlData.h"
#include "T_H_drive.h"
#include "sensorPowerCtrl.h"
#include "uart0Hcho.h"
#include "uart1Pm25.h"
#include "usrBuzz.h"
#include "uart2_485.h"
#include "uart3Co2.h"
#include "touchExti.h"
#include "touchI2c.h"
#include "adcTvoc.h"
#include "userRTC.h"
#include "usrJdq.h"

void vInitHw(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    vInit_RTC();
    vInitBuzz();
    vInitUart0Hcho();
    vInitUart1Pm25();
    vInitUart2_485();
    vInitUart3_Co2();
    vInitSi7020Hw();
    vInitAdcTvoc();
    vInitSensorPowerCtrl();
    vInitTouchI2c();
    vInitTouchEXTI();
    tli_config();
    vInitLcdBright();    
    vLcdBrightSetter(LCD_MAX_BRIGHT);
    vSetLcdBright_1000degrees(u16LcdBrightGetter());
    vInitJDQ();
   //test
    vPowerSetter(POWER_ON);
    vSetSensorPower(u8PowerGetter());
}

