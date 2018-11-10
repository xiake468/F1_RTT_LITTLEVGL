#include "sensorPowerCtrl.h"

void vInitSensorPowerCtrl(void)
{
    rcu_periph_clock_enable(RCU_GPIOE);
    gpio_init(SENSOR_POWER_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, SENSOR_POWER_PIN);
    CLR_SENSOR_POWER;
}

void vSetSensorPower(uint8_t u8power)
{
    if(u8power==SENSOR_POWER_ON)
    {
        SET_SENSOR_POWER;
    }
    else
    {
        CLR_SENSOR_POWER;
    }
}
