#include "uart1Pm25.h"

void vInitUart1Pm25(void)
{
    rcu_periph_clock_enable(RCU_AF);
    nvic_irq_enable(USART1_IRQn, 3, 1);
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOD);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    /* USART configure */
    usart_deinit(USART1);
    gpio_pin_remap_config(GPIO_USART1_REMAP, ENABLE);
    usart_baudrate_set(USART1, 9600U);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_enable(USART1);
    usart_interrupt_enable(USART1, USART_INT_RBNE);
    usart_interrupt_disable(USART1, USART_INT_ERR);
}



