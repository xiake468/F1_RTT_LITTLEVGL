#include "usrIdleTask.h"
#include "T_H_drive.h"
#include "ControlData.h"
#include "usrTimer.h"
#include "co2_Task.h"
#include "adcTvoc.h"
#include <rtdevice.h>

void usrIdleTask(void)
{
        if(u8PowerGetter() == POWER_ON)
        {
            if(co2TxWorkQueue != RT_NULL)
            {
                rt_workqueue_dowork(co2TxWorkQueue, &askCo2ValueWork);
            }

            vGetTvocValue();
            vSi7020Read();
            vDelay_ms(1500);
        }
}

void vUsrIdleInit(void)
{
    rt_thread_idle_sethook(usrIdleTask);
}



