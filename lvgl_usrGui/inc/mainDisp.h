#ifndef   __MAIN_DISP_H__
#define  __MAIN_DISP_H__

#include <rthw.h>
#include <rtthread.h>
#include "gd32f20x.h"
#include "main.h"
#include "lvgl.h"

void vInitMainDisp(void);
void vFreshSensorDispData(void);
void vFreshTime(lv_obj_t * timeLabel);


#endif






