#include "uart0Hcho.h"

void vInitUart0Hcho(void)
{
    rcu_periph_clock_enable(RCU_AF);
    nvic_irq_enable(USART0_IRQn, 3, 1);
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    /* USART configure */
    usart_deinit(USART0);
    
    usart_baudrate_set(USART0, 9600U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
    usart_interrupt_enable(USART0, USART_INT_RBNE);
    usart_interrupt_disable(USART0, USART_INT_ERR);
}




