#include "usrJdq.h"

void vInitJDQ(void)
{
    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_init(FAN_L_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, FAN_L_PIN);
    
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(FAN_M_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, FAN_M_PIN);
   
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(FAN_H_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, FAN_H_PIN);
   vSetFanLevel(0);
}

void vSetFanLevel(uint8_t u8Fanlevel)
{   
  
        switch(u8Fanlevel)
        {
        case 0:
        {
            SET_FAN_0();
        }
        break;

        case 1:
        {
            SET_FAN_1();
        }
        break;

        case 2:
        {
            SET_FAN_2();
        }
        break;

        case 3:
        {
            SET_FAN_3();
        }
        break;

        default:
        {
            SET_FAN_0();
        }
    }
}




