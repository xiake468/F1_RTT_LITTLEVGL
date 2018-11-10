#include "usrGuiPort.h"
#include "lvgl.h"
#include "driveLcd.h"
#include "touchI2c.h"

static void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
static void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
static void ex_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color);
static bool ex_tp_read(lv_indev_data_t * data);

#define TOUCH_QUEUE_SIZE 1
static rt_mq_t touchExtiQueue = RT_NULL;
static void vInitExtiQueue(void)
{
    touchExtiQueue = rt_mq_create("touchQ",
                                  sizeof(uint8_t),
                                  TOUCH_QUEUE_SIZE,
                                  RT_IPC_FLAG_FIFO);
}

void vInitGuiPort(void)
{
    vInitExtiQueue();
    lv_init();

    lv_tick_inc(5);
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.disp_flush = ex_disp_flush;

    disp_drv.disp_fill = ex_disp_fill;
    disp_drv.disp_map = ex_disp_map;
    lv_disp_drv_register(&disp_drv);

    //init touch
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = ex_tp_read;
    lv_indev_drv_register(&indev_drv);
    
}

static void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    int32_t x;
    int32_t y;

    for(y = y1; y <= y2; y++)
    {
        for(x = x1; x <= x2; x++)
        {
            /* Put a pixel to the display. For example: */
            put_px(x, y, *color_p);
            color_p++;
        }
    }

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_flush_ready();
}

static void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    int32_t x;
    int32_t y;

    for(y = y1; y <= y2; y++)
    {
        for(x = x1; x <= x2; x++)
        {
            /* Put a pixel to the display. For example: */
            put_px(x, y, *color_p);
            color_p++;
        }
    }
}

static void ex_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color)
{
    int32_t x;
    int32_t y;

    for(y = y1; y <= y2; y++)
    {
        for(x = x1; x <= x2; x++)
        {
            /* Put a pixel to the display. For example: */
            put_px(x, y, color);
        }
    }

    (void)color; /*Just to avid warnings*/
}

void EXTI1_IRQHandler(void)
{
    if(exti_interrupt_flag_get(EXTI_1))
    {
        uint8_t u8Temp = 0x01;
        rt_interrupt_enter();
        exti_interrupt_flag_clear(EXTI_1);
        rt_mq_send(touchExtiQueue, &u8Temp, 1);
        rt_interrupt_leave();
    }
}

static bool ex_tp_read(lv_indev_data_t * data)
{
    uint8_t u8Temp = 0;

    if(RT_EOK == rt_mq_recv(touchExtiQueue, &u8Temp, 1,  0))
    {
        data->state = LV_INDEV_STATE_PR ;// LV_INDEV_STATE_PR
        data->point.x = FT6336_read_touch1_x();
        data->point.y = FT6336_read_touch1_y();
    }
    else
    {
         data->state =LV_INDEV_STATE_REL;
    }

    (void)data; /*Just to avid warnings*/

    return false;   /*false: no more data to read because we are no buffering*/
}


