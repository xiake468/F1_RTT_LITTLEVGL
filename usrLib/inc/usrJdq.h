#ifndef __USR_JDQ_H__
#define __USR_JDQ_H__

#include <rthw.h>
#include <rtthread.h>
#include "gd32f20x.h"
#include "main.h"

#define FAN_H_PORT GPIOB
#define FAN_H_PIN  GPIO_PIN_15
#define FAN_M_PORT GPIOB
#define FAN_M_PIN  GPIO_PIN_14
#define FAN_L_PORT GPIOD
#define FAN_L_PIN  GPIO_PIN_13

#define SET_FAN_H  GPIO_BOP(FAN_H_PORT) = FAN_H_PIN
#define CLR_FAN_H  GPIO_BC(FAN_H_PORT) =  FAN_H_PIN
#define SET_FAN_M  GPIO_BOP(FAN_M_PORT) = FAN_M_PIN
#define CLR_FAN_M  GPIO_BC(FAN_M_PORT) =  FAN_M_PIN
#define SET_FAN_L  GPIO_BOP(FAN_L_PORT) = FAN_L_PIN
#define CLR_FAN_L  GPIO_BC(FAN_L_PORT) =  FAN_L_PIN

#define SET_FAN_0(){\
    CLR_FAN_H;\
    CLR_FAN_M;\
    CLR_FAN_L;\
}

#define SET_FAN_1(){\
    CLR_FAN_H;\
    CLR_FAN_M;\
    SET_FAN_L;\
}

#define SET_FAN_2(){\
    CLR_FAN_H;\
    SET_FAN_M;\
    CLR_FAN_L;\
}

#define SET_FAN_3(){\
    SET_FAN_H;\
    CLR_FAN_M;\
    CLR_FAN_L;\
}


void vInitJDQ(void);
void vSetFanLevel(uint8_t u8Fanlevel);

#endif



