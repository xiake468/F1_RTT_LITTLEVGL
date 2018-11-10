#ifndef   __CO2_TASK_H__
#define  __CO2_TASK_H__

#include <rthw.h>
#include <rtthread.h>
#include "gd32f20x.h"
#include "main.h"

void vInitUart3Co2Task(void);

extern struct rt_workqueue * co2TxWorkQueue;
extern struct rt_work askCo2ValueWork;

#endif




