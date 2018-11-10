#ifndef __UART2_485_H__
#define __UART2_485_H__

#include <rthw.h>
#include <rtthread.h>
#include "gd32f20x.h"
#include "main.h"

#define UART2_485E_PORT  GPIOD
#define UART2_485E_PIN   GPIO_PIN_14

#define SET_485E  GPIO_BOP(UART2_485E_PORT) = UART2_485E_PIN
#define CLR_485E  GPIO_BC(UART2_485E_PORT) = UART2_485E_PIN

void vInitUart2_485(void);

#endif


