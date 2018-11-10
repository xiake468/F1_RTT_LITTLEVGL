#include "touchExti.h"
#include "usrTimer.h"

static void vInitTouchExit(void);
static void vInitTouchRst(void);

void vInitTouchEXTI(void)
{
    vInitTouchRst();
    vInitTouchExit();
}

static void vInitTouchExit(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);

    /* configure button pin as input */
    gpio_init(TOUCH_IT_PORT, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, TOUCH_IT_PIN);

    /* enable and set key EXTI interrupt to the lowest priority */
    nvic_irq_enable(EXTI1_IRQn, 1, 0);

    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOC, GPIO_PIN_SOURCE_1);

    /* configure key EXTI line */
    exti_init(EXTI_1, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_1);
}

static void vInitTouchRst(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_0);

    GPIO_BC(GPIOC) = GPIO_PIN_0;
    vDelay_ms(50);
    GPIO_BOP(GPIOC) = GPIO_PIN_0;
}

