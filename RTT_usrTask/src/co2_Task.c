#include "co2_Task.h"
#include "SensorData.h"
#include "rtdevice.h"
#include "usrUtility.h"
#include <rtdevice.h>

static void vHandleCo2Task(void* parameter);
static uint8_t u8Co2Check(uint8_t* pU8Ptr, uint8_t u8Len);

struct rt_work askCo2ValueWork;
static void askCo2Value(struct rt_work* work, void* work_data);

#define  CO2_RX_QUEUE_SIZE  32
static rt_mq_t co2RxQueue = RT_NULL;
static rt_thread_t co2RxTask = RT_NULL;

struct rt_workqueue * co2TxWorkQueue = RT_NULL;

void vInitUart3Co2Task(void)
{
    co2RxQueue = rt_mq_create("co2TxQ",
                              sizeof(uint8_t),
                              CO2_RX_QUEUE_SIZE,
                              RT_IPC_FLAG_FIFO);
    co2RxTask = rt_thread_create("co2RxT",
                                 vHandleCo2Task, RT_NULL,
                                 300, 5, 5);

    if(co2RxTask != RT_NULL)
        rt_thread_startup(co2RxTask);
    
    co2TxWorkQueue = rt_workqueue_create("co2TxW", 300, 4);

    if(co2TxWorkQueue != RT_NULL)
    {
        rt_work_init(&askCo2ValueWork, askCo2Value, RT_NULL);
        rt_workqueue_dowork(co2TxWorkQueue, &askCo2ValueWork);
    }
}

void UART3_IRQHandler(void)
{
    rt_interrupt_enter();

    if(usart_flag_get(UART3, USART_FLAG_RBNE))
    {
        uint8_t u8Temp = usart_data_receive(UART3);
        rt_mq_send(co2RxQueue, &u8Temp, 1);
        usart_flag_clear(UART3, USART_FLAG_RBNE);
    }

    rt_interrupt_leave();
}

enum CO2_STEP {CO2_HEAD = 0, CO2_LEN, CO2_DATA, CO2_CS};
static uint8_t l_u8Co2Data[15];
static void vHandleCo2Task(void* parameter)
{
    uint8_t u8Co2Temp = 0;
    uint8_t u8Co2DataLen = 0;

    while(1)
    {
        uint8_t u8Co2_step = CO2_HEAD;
        uint8_t u8Index = 0;
        rt_thread_delay(10); 
        while(RT_EOK == rt_mq_recv(co2RxQueue, &u8Co2Temp, 1,  2))
        {
            switch(u8Co2_step)
            {
            case CO2_HEAD:
            {
                if(u8Co2Temp == 0x16)
                {
                    u8Co2_step = CO2_LEN;
                    l_u8Co2Data[u8Index] = u8Co2Temp;
                    u8Index++;
                }
                else
                {
                    u8Co2_step = CO2_HEAD;
                    u8Index = 0;
                }
            }
            break;

            case CO2_LEN:
            {
                if(u8Co2Temp <= 12)
                {
                    u8Co2DataLen = u8Co2Temp;
                    u8Co2_step = CO2_DATA;
                    l_u8Co2Data[u8Index] = u8Co2Temp;
                    u8Index++;
                }
                else
                {
                    u8Co2_step = CO2_HEAD;
                    u8Index = 0;
                }

            }
            break;

            case CO2_DATA:
            {
                l_u8Co2Data[u8Index] = u8Co2Temp;
                u8Index++;

                if(u8Index >= u8Co2DataLen + 3)
                {
                    u8Co2_step = CO2_CS;
                }
            }
            break;

            case CO2_CS:
            {
                l_u8Co2Data[u8Index] = u8Co2Temp;
                u8Index++;

                if(u8Co2Temp == u8Co2Check(l_u8Co2Data, u8Co2DataLen + 2))
                {
                    if(l_u8Co2Data[2] == 0x02)
                    {
                        vCo2Setter(u16ConvBtoL(&l_u8Co2Data[3]));
                    }
                }

                u8Co2_step = CO2_HEAD;
                u8Index = 0;
            }
            break;

            default:
            {
                u8Co2_step = CO2_HEAD;
                u8Index = 0;
            }
            break;
            }
        }
    }
}

static uint8_t u8Co2Check(uint8_t* pU8Ptr, uint8_t u8Len)
{
    uint8_t u8Tmp = 0;

    for(uint8_t i = 0; i < u8Len; i++)
    {
        u8Tmp += (*pU8Ptr + i);
    }

    u8Tmp = 256 - u8Tmp;
    return u8Tmp;
}

void askCo2Value(struct rt_work* work, void* work_data)
{
    rt_thread_delay(RT_TICK_PER_SECOND / 50);
}

