#ifndef __SENSOR_POWER_CTRL_H__
#define __SENSOR_POWER_CTRL_H__

#include <rthw.h>
#include <rtthread.h>
#include "gd32f20x.h"
#include "main.h"

#define SENSOR_POWER_PORT   GPIOE
#define SENSOR_POWER_PIN    GPIO_PIN_2

#define SET_SENSOR_POWER  GPIO_BOP(SENSOR_POWER_PORT) = SENSOR_POWER_PIN
#define CLR_SENSOR_POWER  GPIO_BC(SENSOR_POWER_PORT) =  SENSOR_POWER_PIN

enum SENSOR_POWER{SENSOR_POWER_OFF=0,SENSOR_POWER_ON};

void vInitSensorPowerCtrl(void);
void vSetSensorPower(uint8_t power);


#endif




