#include "pm25Task.h"
#include "SensorData.h"
#include "rtdevice.h"
#include "usrUtility.h"

static void vHandlePm25Task(void* parameter);
static uint16_t u16SumCheckMethod(uint8_t * pU8Ptr, uint8_t u8Len);

#define  PM25_QUEUE_SIZE 96
static rt_mq_t pm25Queue = RT_NULL;
static rt_thread_t pm25Task = RT_NULL;

void vInitUart1Pm25Task(void)
{
    pm25Queue = rt_mq_create("pm25Q",
                             sizeof(uint8_t),
                             PM25_QUEUE_SIZE,
                             RT_IPC_FLAG_FIFO);
    pm25Task = rt_thread_create("pm25T",
                                vHandlePm25Task, RT_NULL,
                                300, 5, 5);

    if(pm25Task != RT_NULL)
        rt_thread_startup(pm25Task);
}

void USART1_IRQHandler(void)
{
    rt_interrupt_enter();

    if(usart_flag_get(USART1, USART_FLAG_RBNE))
    {
        uint8_t u8Temp = usart_data_receive(USART1);
        rt_mq_send(pm25Queue, &u8Temp, 1);
        usart_flag_clear(USART1, USART_FLAG_RBNE);
    }

    rt_interrupt_leave();
}

enum PM25_STEP {PM25_HEAD1 = 0, PM25_HEAD2, PM25_LEN1, PM25_LEN2, PM25_DATA, PM25_CRC_1, PM25_CRC_2};
static uint8_t l_u8Pm25Data[32];
static void vHandlePm25Task(void* parameter)
{
    uint8_t u8Pm25Temp = 0;
    uint8_t u8Pm25DataLen = 0;
    uint16_t u16SumCheck = 0;

    while(1)
    {
        uint8_t u8Pm25_step = PM25_HEAD1;
        uint8_t u8Index = 0;

        while(RT_EOK == rt_mq_recv(pm25Queue, &u8Pm25Temp, 1,  2))
        {
            switch(u8Pm25_step)
            {
            case PM25_HEAD1:
            {
                l_u8Pm25Data[u8Index] = u8Pm25Temp;
                u8Index++;

                if(u8Pm25Temp == 0x42)
                {
                    u8Pm25_step = PM25_HEAD2;
                }
                else
                {
                    u8Index = 0;
                    u8Pm25_step = PM25_HEAD1;
                }
            }
            break;

            case PM25_HEAD2:
            {
                l_u8Pm25Data[u8Index] = u8Pm25Temp;
                u8Index++;

                if(u8Pm25Temp == 0x4D)
                {
                    u8Pm25_step = PM25_LEN1;
                }
                else
                {
                    u8Pm25_step = PM25_HEAD1;
                    u8Index = 0;
                }
            }
            break;

            case PM25_LEN1:
            {
                l_u8Pm25Data[u8Index] = u8Pm25Temp;
                u8Index++;
                if(u8Pm25Temp == 0x00)
                {
                    u8Pm25_step = PM25_LEN2;
                }
                else
                {
                    u8Pm25_step = PM25_HEAD1;
                    u8Index = 0;
                }
            }
            break;

            case PM25_LEN2:
            {
                l_u8Pm25Data[u8Index] = u8Pm25Temp;
                u8Index++;
                if(u8Pm25Temp <= 28)
                {
                    u8Pm25DataLen = u8Pm25Temp;
                    u8Pm25_step = PM25_DATA;
                }
                else
                {
                    u8Pm25_step = PM25_HEAD1;
                    u8Index = 0;
                }
            }
            break;

            case PM25_DATA:
            {
                l_u8Pm25Data[u8Index] = u8Pm25Temp;
                u8Index++;

                if(u8Index >= u8Pm25DataLen + 2)
                    u8Pm25_step = PM25_CRC_1;

            }
            break;

            case PM25_CRC_1:
            {
                u16SumCheck = u16SumCheckMethod(l_u8Pm25Data, u8Pm25DataLen + 2);
                l_u8Pm25Data[u8Index] = u8Pm25Temp;
                u8Index++;

                if((uint8_t)(u16SumCheck >> 8) == u8Pm25Temp)
                {
                    u8Pm25_step = PM25_CRC_2;
                }
                else
                {
                    u8Pm25_step = PM25_HEAD1;
                    u8Index = 0;
                }
            }
            break;

            case PM25_CRC_2:
            {
                l_u8Pm25Data[u8Index] = u8Pm25Temp;
                u8Index++;
                if((uint8_t)(u16SumCheck) == u8Pm25Temp)
                {
                    vPm1Setter(u16ConvBtoL(&l_u8Pm25Data[6]));
                    vPm25Setter(u16ConvBtoL(&l_u8Pm25Data[8]));
                    vPm10Setter(u16ConvBtoL(&l_u8Pm25Data[10]));
                    vPm1_Num_PerL_Setter(10*u16ConvBtoL(&l_u8Pm25Data[20]));
                    vPm25_Num_PerL_Setter(10*u16ConvBtoL(&l_u8Pm25Data[22]));
                    vPm10_Num_PerL_Setter(10*u16ConvBtoL(&l_u8Pm25Data[26]));
                }

                u8Pm25_step = PM25_HEAD1;
                u8Index = 0;
            }
            break;

            default:
            {
                u8Pm25_step = PM25_HEAD1;
                u8Index = 0;
            }
            break;
            }

        }
    }
}

static uint16_t u16SumCheckMethod(uint8_t * pU8Ptr, uint8_t u8Len)
{
    uint16_t u16SumCheck = 0;

    for(uint8_t i = 0; i < u8Len; i++)
    {
        u16SumCheck += *(pU8Ptr + i);
    }

    return u16SumCheck;
}
