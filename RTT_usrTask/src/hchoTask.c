#include "hchoTask.h"
#include "SensorData.h"
#include "usrUtility.h"

static void vHandleHchoTask(void* parameter);
static uint8_t FucCheckSum(uint8_t *i, uint8_t ln);

#define  HCHO_QUEUE_SIZE 27
static rt_mq_t hchoQueue = RT_NULL;
static rt_thread_t hchoTask = RT_NULL;
void vInitUart0HchoTask(void)
{
    hchoQueue = rt_mq_create("hchoQ",
                             sizeof(uint8_t),
                             HCHO_QUEUE_SIZE,
                             RT_IPC_FLAG_FIFO);
    hchoTask = rt_thread_create("hchoT",
                                vHandleHchoTask, RT_NULL,
                                300, 5, 5);

    if(hchoTask != RT_NULL)
        rt_thread_startup(hchoTask);
    //vUsrIdleInit();
}

void USART0_IRQHandler(void)
{
    rt_interrupt_enter();

    if(usart_flag_get(USART0, USART_FLAG_RBNE))
    {
        uint8_t u8Temp = usart_data_receive(USART0);
        rt_mq_send(hchoQueue, &u8Temp, 1);
        usart_flag_clear(USART0, USART_FLAG_RBNE);
    }

    rt_interrupt_leave();
}

enum HCHO_STEP {HCHO_HEAD = 0, HCHO_CHAR1, HCHO_CHAR2, HCHO_CHAR3, HCHO_DATA, HCHO_CHAR4, HCHO_CHAR5, HCHO_CRC};
static uint8_t l_u8HchoData[9];
void vHandleHchoTask(void* parameter)
{
    uint8_t u8HchoTemp = 0;

    while(1)
    {
        uint8_t u8Hcho_step = HCHO_HEAD;
        uint8_t u8Index = 0;

        while(RT_EOK == rt_mq_recv(hchoQueue, &u8HchoTemp, 1,  2))
        {
            switch(u8Hcho_step)
            {
            case  HCHO_HEAD:
            {
                l_u8HchoData[u8Index] = u8HchoTemp;
                u8Index++;

                if(u8HchoTemp == 0XFF)
                {
                    u8Hcho_step = HCHO_CHAR1;
                }
                else
                {
                    u8Hcho_step = HCHO_HEAD;
                    u8Index = 0;
                }
            }
            break;

            case HCHO_CHAR1:
            {
                l_u8HchoData[u8Index] = u8HchoTemp;
                u8Index++;

                if(u8HchoTemp == 0X17)
                {
                    u8Hcho_step = HCHO_CHAR2;

                }
                else
                {
                    u8Hcho_step = HCHO_HEAD;
                    u8Index = 0;
                }
            }
            break;

            case HCHO_CHAR2:
            {
                l_u8HchoData[u8Index] = u8HchoTemp;
                u8Index++;

                if(u8HchoTemp == 0X04)
                {
                    u8Hcho_step = HCHO_CHAR3;
                }
                else
                {
                    u8Hcho_step = HCHO_HEAD;
                    u8Index = 0;
                }
            }
            break;

            case HCHO_CHAR3:
            {
                l_u8HchoData[u8Index] = u8HchoTemp;
                u8Index++;

                if(u8HchoTemp == 0X00)
                {
                    u8Hcho_step = HCHO_DATA;
                }
                else
                {
                    u8Hcho_step = HCHO_HEAD;
                    u8Index = 0;
                }
            }
            break;

            case HCHO_DATA:
            {
                l_u8HchoData[u8Index] = u8HchoTemp;
                u8Index++;

                if(u8Index >= 6)
                    u8Hcho_step = HCHO_CHAR4;
            }
            break;

            case HCHO_CHAR4:
            {
                l_u8HchoData[u8Index] = u8HchoTemp;
                u8Index++;

                if(u8HchoTemp == 0X13)
                {
                    u8Hcho_step = HCHO_CHAR5;
                }
                else
                {
                    u8Hcho_step = HCHO_HEAD;
                    u8Index = 0;
                }
            }
            break;

            case HCHO_CHAR5:
            {
                l_u8HchoData[u8Index] = u8HchoTemp;
                u8Index++;

                if(u8HchoTemp == 0X88)
                {
                    u8Hcho_step = HCHO_CRC;
                }
                else
                {
                    u8Hcho_step = HCHO_HEAD;
                    u8Index = 0;
                }
            }
            break;

            case HCHO_CRC:
            {
                l_u8HchoData[u8Index] = u8HchoTemp;
                u8Index++;
                if(u8HchoTemp == FucCheckSum(l_u8HchoData, 9))
                {
                       vHchoSetter(u16ConvBtoL(&l_u8HchoData[4]));
                }                
                u8Hcho_step = HCHO_HEAD;
                u8Index=0;
            }
            break;

            default:
            {
                u8Hcho_step = HCHO_HEAD;
                u8Index=0;
            }
            break;
            }
        }
    }
}

static uint8_t FucCheckSum(uint8_t *i, uint8_t ln)
{
    uint8_t j, tempq = 0;
    i += 1;

    for(j = 0; j < (ln - 2); j++)
    {
        tempq += *i;
        i++;
    }

    tempq = (~tempq) + 1;
    return(tempq);
}


