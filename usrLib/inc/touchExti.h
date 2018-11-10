#ifndef __TOUCH_EXTI_H__
#define __TOUCH_EXTI_H__

#include <rthw.h>
#include <rtthread.h>
#include "gd32f20x.h"
#include "main.h"

#define TOUCH_IT_PORT  GPIOC
#define TOUCH_IT_PIN  GPIO_PIN_1

void vInitTouchEXTI(void);

#endif





