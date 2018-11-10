#include "uart2_485.h"

static void vInitUart2_485E(void)
{
    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_init(UART2_485E_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, UART2_485E_PIN);
    CLR_485E;
}

void vInitUart2_485(void)
{
    vInitUart2_485E();
    rcu_periph_clock_enable(RCU_AF);
    nvic_irq_enable(USART2_IRQn, 1, 0);
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOD);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART2);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    /* USART configure */
    usart_deinit(USART2);
    gpio_pin_remap_config(GPIO_USART2_FULL_REMAP, ENABLE);
    usart_baudrate_set(USART2, 9600U);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    usart_enable(USART2);
    usart_interrupt_enable(USART2, USART_INT_RBNE);
    usart_interrupt_disable(USART2, USART_INT_ERR);
}







