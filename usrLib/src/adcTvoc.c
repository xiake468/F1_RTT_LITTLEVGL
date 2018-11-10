#include "adcTvoc.h"
#include "usrTimer.h"
#include "SensorData.h"
#include "stdlib.h"

static uint16_t adc_value;

static void vInitRcu(void);
static void vInitAdcGpio(void);
static void vInitAdcToDma(void);
static void vInitAdc(void);
static uint16_t u16AdcFilter(uint16_t u16Data);

void vInitAdcTvoc(void)
{
    vInitRcu();
    vInitAdcGpio();
    vInitAdcToDma();
    vInitAdc();
}

static void vInitRcu(void)
{
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable DMA0 clock */
    rcu_periph_clock_enable(RCU_DMA0);
    /* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
}

static void vInitAdcGpio(void)
{
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
}

static void vInitAdcToDma(void)
{
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;

    /* ADC DMA_channel configuration */
    dma_deinit(DMA0, DMA_CH0);

    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr  = (uint32_t)(&adc_value);
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_DISABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
    dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number       = 1;
    dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH0, dma_data_parameter);
    dma_circulation_enable(DMA0, DMA_CH0);

    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);
}

static void vInitAdc(void)
{
    /* ADC contineous function enable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* ADC mode config */
    adc_mode_config(ADC_MODE_FREE);
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);

    /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_14, ADC_SAMPLETIME_55POINT5);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);

    /* enable ADC interface */
    adc_enable(ADC0);
    vDelay_ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);

    /* ADC DMA function enable */
    adc_dma_mode_enable(ADC0);
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}

void vGetTvocValue(void)
{
    float fTmpTVOC;
    float fK;
    //计算TVOC MG值
    //码值0对应0，,4090对应9.999

    fTmpTVOC = u16AdcFilter(adc_value) * 9.999 / 4090.0;
    //计算K
    //0对应K为0,9.999对应K为1.5
    fK =  fTmpTVOC * 1.5 / 9.999;

    fTmpTVOC *= fK;

    vTvocSetter(fTmpTVOC * 1000);
}

int compInc(const void *a, const void *b)
{
    return *(uint16_t *)b - *(uint16_t *)a;
}

#define   ADCFilterMax     5
static uint16_t   l_u16ADCFilterCnt = 0;
static uint16_t   l_u16ADCFilterBuf[ADCFilterMax];

static uint16_t u16AdcFilter(uint16_t u16Data)
{
    l_u16ADCFilterBuf[l_u16ADCFilterCnt++] = u16Data;

    if(l_u16ADCFilterCnt >= ADCFilterMax)
        l_u16ADCFilterCnt = 0;

    qsort(l_u16ADCFilterBuf, ADCFilterMax, sizeof(l_u16ADCFilterBuf[0]), compInc);
    return l_u16ADCFilterBuf[ADCFilterMax / 2 + 1];
}

