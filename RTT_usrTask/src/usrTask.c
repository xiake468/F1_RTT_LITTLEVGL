#include "usrTask.h"
#include "usrIdleTask.h"
#include "hchoTask.h"
#include "pm25Task.h"
#include "co2_Task.h"
#include "usrGuiTask.h"

void vInitTask(void)
{
    vInitUsrGuiTask();
    vInitUart0HchoTask();
    vInitUart1Pm25Task();
    vInitUart3Co2Task();
    vUsrIdleInit();
}




