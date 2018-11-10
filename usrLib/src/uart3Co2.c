#include "uart3Co2.h"

void vInitUart3_Co2(void)
{
    rcu_periph_clock_enable(RCU_AF);
    nvic_irq_enable(UART3_IRQn, 3, 1);
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_UART3);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    /* USART configure */
    usart_deinit(UART3);
    gpio_pin_remap1_config(GPIO_PCF5, GPIO_PCF5_UART3_REMAP, ENABLE);
    usart_baudrate_set(UART3, 9600U);
    usart_receive_config(UART3, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
    usart_enable(UART3);
    usart_interrupt_enable(UART3, USART_INT_RBNE);
    usart_interrupt_disable(UART3, USART_INT_ERR);	
}








