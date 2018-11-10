#include "driveLcd.h"

static void tli_gpio_config(void);

static volatile uint8_t image_background[LCD_HIGHT][LCD_WIDTH * 2] = {0};

void tli_config(void)
{
    tli_parameter_struct               tli_init_struct;
    tli_layer_parameter_struct         tli_layer_init_struct;

    rcu_periph_clock_enable(RCU_TLI);
    tli_gpio_config();

    /* configure the TLI clock to generate LCD clock */
    rcu_pllt_config(RCU_PLLTSRC_IRC8M);

    if(ERROR == rcu_pllt_vco_config(8, 90, 4 ))
    {
        while(1);
    }

    rcu_tli_clock_config(RCU_CKTLI_CKPLLTR_DIV2);

    rcu_osci_on(RCU_PLLT_CK);

    if(ERROR == rcu_osci_stab_wait(RCU_PLLT_CK))
    {
        while(1);
    }

    /* configure TLI parameter struct */
    tli_init_struct.signalpolarity_hs = TLI_HSYN_ACTLIVE_HIGHT;
    tli_init_struct.signalpolarity_vs = TLI_HSYN_ACTLIVE_HIGHT;
    tli_init_struct.signalpolarity_de = TLI_DE_ACTLIVE_HIGHT;
    tli_init_struct.signalpolarity_pixelck = TLI_PIXEL_CLOCK_INVERTEDTLI;

    /* LCD display timing configuration */
    tli_init_struct.synpsz_hpsz = LCD_HPW - 1;
    tli_init_struct.synpsz_vpsz = LCD_VPW - 1;
    tli_init_struct.backpsz_hbpsz = LCD_HPS - 1;
    tli_init_struct.backpsz_vbpsz = LCD_VPS - 1;
    tli_init_struct.activesz_hasz = LCD_HPS + LCD_WIDTH - 1;
    tli_init_struct.activesz_vasz = LCD_VPS + LCD_HIGHT - 1;
    tli_init_struct.totalsz_htsz = LCD_HT;
    tli_init_struct.totalsz_vtsz = LCD_VT;
    /* LCD background color configure */
    tli_init_struct.backcolor_red = 0x00;
    tli_init_struct.backcolor_green = 0x00;
    tli_init_struct.backcolor_blue = 0x00;
    tli_init(&tli_init_struct);

//    /* tli layer1 configuration */
//    /* tli window size configuration */
//    tli_layer_init_struct.layer_window_leftpos = HSW + HBP;
//    tli_layer_init_struct.layer_window_rightpos = HSW + HBP + LCD_WIDTH - 1;
//    tli_layer_init_struct.layer_window_toppos = VSW + VBP;
//    tli_layer_init_struct.layer_window_bottompos = VSW + VBP + LCD_HIGHT - 1;
//    /* tli window pixel format configuration */
//    tli_layer_init_struct.layer_ppf = LAYER_PPF_ARGB1555;
//    /* tli window specified alpha configuration */
//    tli_layer_init_struct.layer_sa = 0xff;
//    /* tli window blend configuration */
//    tli_layer_init_struct.layer_acf1 = LAYER_ACF1_PASA;
//    tli_layer_init_struct.layer_acf2 = LAYER_ACF1_PASA;
//    /* tli layer default alpha R,G,B value configuration */
//    tli_layer_init_struct.layer_default_alpha = 0;
//    tli_layer_init_struct.layer_default_blue = 0;
//    tli_layer_init_struct.layer_default_green = 0;
//    tli_layer_init_struct.layer_default_red = 0;
//    /* tli layer frame buffer base address configuration */
//    tli_layer_init_struct.layer_frame_bufaddr = (uint32_t)&gImage_1;
//    tli_layer_init_struct.layer_frame_line_length = ((320 * 2) + 3);
//    tli_layer_init_struct.layer_frame_buf_stride_offset = (320 * 2);
//    tli_layer_init_struct.layer_frame_total_line_number = 240;
//    tli_layer_init(LAYER1, &tli_layer_init_struct);

    /* tli layer0 configuration */
    tli_layer_init_struct.layer_window_leftpos = LCD_HPS;
    tli_layer_init_struct.layer_window_rightpos = LCD_HPS  + LCD_WIDTH - 1 ;
    tli_layer_init_struct.layer_window_toppos = LCD_VPS;
    tli_layer_init_struct.layer_window_bottompos = LCD_VPS + LCD_HIGHT - 1;
    tli_layer_init_struct.layer_ppf = LAYER_PPF_RGB565;
    /* tli window specified alpha configuration */
    tli_layer_init_struct.layer_sa = 0xff;
    /* tli layer default alpha R,G,B value configuration */
    tli_layer_init_struct.layer_default_blue = 0x00;
    tli_layer_init_struct.layer_default_green = 0;
    tli_layer_init_struct.layer_default_red = 0;
    tli_layer_init_struct.layer_default_alpha = 0;
    /* tli window blend configuration */
    tli_layer_init_struct.layer_acf1 = LAYER_ACF1_SA;
    tli_layer_init_struct.layer_acf2 = LAYER_ACF2_SA;
    /* tli layer frame buffer base address configuration */
    tli_layer_init_struct.layer_frame_bufaddr = (uint32_t)(&image_background);
    tli_layer_init_struct.layer_frame_line_length = 320 * 2 + 3;
    tli_layer_init_struct.layer_frame_buf_stride_offset = 320 * 2;
    tli_layer_init_struct.layer_frame_total_line_number = 240;
    tli_layer_init(LAYER0, &tli_layer_init_struct);

    tli_reload_config(TLI_REQUEST_RELOAD_EN);
    //tli_dither_config(TLI_DITHER_ENABLE);
    /* layer0 and layer1 enable */
    tli_layer_enable(LAYER0);
    //tli_layer_enable(LAYER1);
    /* reload layer0 and layer1 configuration */
    tli_reload_config(TLI_REQUEST_RELOAD_EN);
    tli_enable();
}

static void tli_gpio_config(void)
{
    /* GPIO clock enable */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_AF);

    /* TLI GPIO pin remap configuration */


    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_12 | GPIO_PIN_11 | GPIO_PIN_6);

    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_1 | GPIO_PIN_0);

    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);

    gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_3);

    gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_11);

    gpio_pin_remap1_config(GPIO_PCF3, GPIO_PCF3_TLI_B3_PD10_REMAP | GPIO_PCF3_TLI_VSYNC_PA4_REMAP | GPIO_PCF3_TLI_B7_PB9_REMAP |
                           GPIO_PCF3_TLI_CLK_PE14_REMAP | GPIO_PCF3_TLI_R7_PE15_REMAP | GPIO_PCF3_TLI_B6_PB8_REMAP |
                           GPIO_PCF3_TLI_B5_PA3_REMAP | GPIO_PCF3_TLI_B4_PE12_REMAP | GPIO_PCF3_TLI_G7_PD3_REMAP |
                           GPIO_PCF3_TLI_DE_PE13_REMAP | GPIO_PCF3_TLI_HSYNC_PC6_REMAP | GPIO_PCF3_TLI_G6_PC7_REMAP |
                           GPIO_PCF3_TLI_G5_PB11_REMAP | GPIO_PCF3_TLI_G4_PB10_REMAP | GPIO_PCF3_TLI_G3_PE11_REMAP |
                           GPIO_PCF3_TLI_R6_PB1_REMAP | GPIO_PCF3_TLI_R5_PA12_REMAP | GPIO_PCF3_TLI_R4_PA11_REMAP | GPIO_PCF3_TLI_G2_PA6_REMAP | GPIO_PCF3_TLI_R3_PB0_REMAP
                           , ENABLE);
}

void put_px(int32_t x, int32_t y, lv_color_t  color_p)
{
    int32_t tempX_Index=639-x*2;
    int32_t tempY_Index=239-y;
    image_background[tempY_Index][tempX_Index] = (uint8_t)(color_p.full>>8);
    image_background[tempY_Index][tempX_Index-1] = (uint8_t)(color_p.full);
}






