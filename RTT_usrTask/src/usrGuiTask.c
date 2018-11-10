#include "usrGuiTask.h"
#include "lvgl.h"
#include "driveLcd.h"
#include "usrGuiPort.h"
#include "mainDisp.h"

static void vGuiTask(void* parameter);

static rt_thread_t usrGuiTask = RT_NULL;

LV_FONT_DECLARE(msyh20)

void vInitUsrGuiTask(void)
{
    lv_font_add(&msyh20, &lv_font_dejavu_20);
    
    usrGuiTask = rt_thread_create("GuiT",
                                  vGuiTask, RT_NULL,
                                  2000, 4, 10);

    if(usrGuiTask != RT_NULL)
        rt_thread_startup(usrGuiTask);
}

static void vGuiTask(void* parameter)
{
     vInitGuiPort();
    vInitMainDisp();
    while(1)
    {
        vFreshSensorDispData();
        lv_task_handler();
        rt_thread_delay(5);
        lv_tick_inc(5);
    }
}






