#include "mainDisp.h"
#include "SensorData.h"
#include "ControlData.h"
#include "stdio.h"
#include "userRTC.h"
#include "adjustTime.h"
#include "setDisp.h"
#include "usrJdq.h"

static lv_obj_t* mainScr = NULL;
static lv_obj_t* timeLabel = NULL;
static lv_obj_t * inDoorH_lmeter = NULL;
static  lv_obj_t * inDoorH_Label = NULL;
static  lv_obj_t * inDoorT_lmeter = NULL;
static  lv_obj_t * inDoorT_Label = NULL;
static  lv_obj_t * pm25_lmeter = NULL;
static  lv_obj_t * pm25_Label = NULL;
static  lv_obj_t * autoSw=NULL;
static lv_style_t style_pm25_lmeter;

static lv_res_t mainBtn_click_action(lv_obj_t * btn);
static lv_res_t swAction(lv_obj_t * btn);
static lv_res_t timeBtn_click_action(void);
static lv_res_t setBtn_click_action(void);
static void vModifyPm25Style(void);

LV_FONT_DECLARE(pm25Num)

void vInitMainDisp(void)
{
    mainScr = lv_obj_create(NULL, NULL);
    lv_scr_load(mainScr);
    /*Create a Time button*/
    lv_obj_t * timeBtn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(timeBtn, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 1, 1);
    lv_obj_set_size(timeBtn, 220, 22);
    lv_obj_set_free_num(timeBtn, MAIN_TIME_BUTTON_NUM);   /*Set a unique number for the button*/
    lv_btn_set_action(timeBtn, LV_BTN_ACTION_CLICK, mainBtn_click_action);
    timeLabel = lv_label_create(timeBtn, NULL);
    vFreshTime(timeLabel);
    /*Create a set button*/
    lv_obj_t * setBtn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(setBtn, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, 60, 26);
    lv_obj_set_size(setBtn, 40, 40);
    static lv_style_t style_sym;
    lv_style_copy(&style_sym, &lv_style_plain);
    style_sym.text.color = LV_COLOR_WHITE;
    style_sym.text.font = &lv_font_dejavu_40;       /*The built-in fonts are extended with symbols*/
    lv_obj_t * img_sym = lv_img_create(setBtn, NULL);
    lv_img_set_src(img_sym, SYMBOL_SETTINGS);
    lv_img_set_style(img_sym, &style_sym);
    lv_obj_set_free_num(setBtn, MAIN_SET_BUTTON_NUM);   /*Set a unique number for the button*/
    lv_btn_set_action(setBtn, LV_BTN_ACTION_CLICK, mainBtn_click_action);
    /*Create a Wifi button*/
    lv_obj_t * wifiBtn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(wifiBtn, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, 70, 1);
    lv_obj_set_size(wifiBtn, 30, 28);
    static lv_style_t style_wifiBtn;
    lv_style_copy(&style_wifiBtn, &lv_style_plain);
    style_wifiBtn.text.color = LV_COLOR_GRAY;
    style_wifiBtn.text.font = &lv_font_dejavu_20;       /*The built-in fonts are extended with symbols*/
    lv_obj_t * img_wifi = lv_img_create(wifiBtn, NULL);
    lv_img_set_src(img_wifi, SYMBOL_WIFI);
    lv_img_set_style(img_wifi, &style_wifiBtn);
    lv_obj_set_free_num(wifiBtn, MAIN_WIFI_BUTTON_NUM);   /*Set a unique number for the button*/
    lv_btn_set_action(wifiBtn, LV_BTN_ACTION_CLICK, mainBtn_click_action);
    /*Create a H lmeter*/
    /*Create a simple style with ticker line width*/
    static lv_style_t style_indoorH_lmeter;
    lv_style_copy(&style_indoorH_lmeter, &lv_style_pretty_color);
    style_indoorH_lmeter.line.width = 2;
    style_indoorH_lmeter.line.color = LV_COLOR_SILVER;
    style_indoorH_lmeter.body.main_color = LV_COLOR_HEX(0x91bfed);         /*Light blue*/
    style_indoorH_lmeter.body.grad_color = LV_COLOR_HEX(0x04386c);         /*Dark blue*/

    /*Create the first line meter */
    inDoorH_lmeter = lv_lmeter_create(lv_scr_act(), NULL);
    lv_lmeter_set_range(inDoorH_lmeter, 0, 100);                   /*Set the range*/
    lv_lmeter_set_value(inDoorH_lmeter, u16IndoorH_Getter() / 10);                     /*Set the current value*/
    lv_lmeter_set_style(inDoorH_lmeter, &style_indoorH_lmeter);           /*Apply the new style*/
    lv_obj_set_size(inDoorH_lmeter, 80, 80);
    lv_obj_align(inDoorH_lmeter, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, -3, 40);

    /*Add a label to show the current value*/
    char txt[10];
    inDoorH_Label = lv_label_create(inDoorH_lmeter, NULL);
    sprintf(txt, "%.1f%%\n \u6e7f\u5ea6", (float)u16IndoorH_Getter() / 10.0);
    lv_label_set_text(inDoorH_Label, txt);
    lv_label_set_style(inDoorH_Label, &lv_style_pretty);
    lv_obj_align(inDoorH_Label, NULL, LV_ALIGN_CENTER, 0, 10);
    /*Create a T lmeter*/
    /*Create a simple style with ticker line width*/
    static lv_style_t style_indoorT_lmeter;
    lv_style_copy(&style_indoorT_lmeter, &lv_style_pretty_color);
    style_indoorT_lmeter.line.width = 2;
    style_indoorT_lmeter.line.color = LV_COLOR_SILVER;
    style_indoorT_lmeter.body.main_color = LV_COLOR_CYAN;         /*Light blue*/
    style_indoorT_lmeter.body.grad_color = LV_COLOR_RED;         /*Dark blue*/


    /*Create the first line meter */
    inDoorT_lmeter = lv_lmeter_create(lv_scr_act(), NULL);
    lv_lmeter_set_range(inDoorT_lmeter, -10, 38);                   /*Set the range*/
    lv_lmeter_set_value(inDoorT_lmeter, s16IndoorT_Getter() / 10);                     /*Set the current value*/
    lv_lmeter_set_style(inDoorT_lmeter, &style_indoorT_lmeter);           /*Apply the new style*/
    lv_obj_set_size(inDoorT_lmeter, 80, 80);
    lv_obj_align(inDoorT_lmeter, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, -3, -40);

    /*Add a label to show the current value*/
    inDoorT_Label = lv_label_create(inDoorT_lmeter, NULL);
    sprintf(txt, "%.1f\u2103\n \u6e29\u5ea6", (float)s16IndoorT_Getter() / 10.0);
    lv_label_set_text(inDoorT_Label, txt);
    lv_label_set_style(inDoorT_Label, &lv_style_pretty);
    lv_obj_align(inDoorT_Label, NULL, LV_ALIGN_CENTER, 0, 10);
    //PM25 lmeter
    lv_style_copy(&style_pm25_lmeter, &lv_style_pretty_color);
    style_pm25_lmeter.line.width = 2;
    style_pm25_lmeter.body.padding.hor = 12;
    style_pm25_lmeter.text.font = &pm25Num;

    /*Create the first line meter */
    pm25_lmeter = lv_lmeter_create(lv_scr_act(), NULL);
    vModifyPm25Style();
    lv_lmeter_set_style(pm25_lmeter, &style_pm25_lmeter);           /*Apply the new style*/
    lv_lmeter_set_scale(pm25_lmeter, 360, 46);
    lv_obj_set_size(pm25_lmeter, 145, 145);
    lv_obj_align(pm25_lmeter, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 95, -1);
    /*Add a label to show the current value*/
    pm25_Label = lv_label_create(pm25_lmeter, NULL);
    sprintf(txt, "%d", u16Pm25Getter());
    lv_label_set_text(pm25_Label, txt);
    lv_label_set_style(pm25_Label, &style_pm25_lmeter);
    /*Create styles for the switch*/
    static lv_style_t bg_style;
    static lv_style_t indic_style;
    static lv_style_t knob_on_style;
    static lv_style_t knob_off_style;
    lv_style_copy(&bg_style, &lv_style_pretty);
    bg_style.body.radius = LV_RADIUS_CIRCLE;
    bg_style.body.padding.ver=-10;
 
    lv_style_copy(&indic_style, &lv_style_pretty_color);
    indic_style.body.radius = LV_RADIUS_CIRCLE;
    indic_style.body.main_color = LV_COLOR_HEX(0x9fc8ef);
    indic_style.body.grad_color = LV_COLOR_HEX(0x9fc8ef);
    indic_style.body.padding.hor = 0;
    indic_style.body.padding.ver = 0;
    
    lv_style_copy(&knob_off_style, &lv_style_pretty);
    knob_off_style.body.radius = LV_RADIUS_CIRCLE;
    
    lv_style_copy(&knob_on_style, &lv_style_pretty_color);
    knob_on_style.body.radius = LV_RADIUS_CIRCLE;

    /*Create a switch and apply the styles*/
    autoSw = lv_sw_create(lv_scr_act(), NULL);
    lv_sw_set_style(autoSw, LV_SW_STYLE_BG, &bg_style);
    lv_sw_set_style(autoSw, LV_SW_STYLE_INDIC, &indic_style);
    lv_sw_set_style(autoSw, LV_SW_STYLE_KNOB_ON, &knob_on_style);
    lv_sw_set_style(autoSw, LV_SW_STYLE_KNOB_OFF, &knob_off_style);
    lv_obj_align(autoSw, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 50, 40);
    lv_obj_set_free_num(autoSw, MAIN_AUTO_SW_NUM);   /*Set a unique number for the button*/
    lv_sw_set_action (autoSw, swAction);
   
}

void vDeleteMainDisp(void)
{
    lv_obj_del(mainScr);
}

static void vModifyPm25Style(void)
{
    if(u16Pm25Getter() <= 35)
    {
        style_pm25_lmeter.text.color = LV_COLOR_LIME;
        style_pm25_lmeter.line.color = LV_COLOR_LIME;
        style_pm25_lmeter.body.main_color = LV_COLOR_YELLOW;         /*START*/
        style_pm25_lmeter.body.grad_color = LV_COLOR_YELLOW;         /*END*/
        lv_lmeter_set_range(pm25_lmeter, 0, 35);
        lv_lmeter_set_value(pm25_lmeter, u16Pm25Getter());
    }
    else if(u16Pm25Getter() <= 75)
    {
        style_pm25_lmeter.text.color = LV_COLOR_YELLOW;
        style_pm25_lmeter.line.color = LV_COLOR_YELLOW;
        style_pm25_lmeter.body.main_color = LV_COLOR_RED;         /*Light blue*/
        style_pm25_lmeter.body.grad_color = LV_COLOR_RED;         /*Dark blue*/
        lv_lmeter_set_range(pm25_lmeter, 35, 75);
        lv_lmeter_set_value(pm25_lmeter, u16Pm25Getter());
    }
    else if(u16Pm25Getter() <= 150)
    {
        style_pm25_lmeter.text.color = LV_COLOR_RED;
        style_pm25_lmeter.line.color = LV_COLOR_RED;
        style_pm25_lmeter.body.main_color = LV_COLOR_PURPLE;         /*Light blue*/
        style_pm25_lmeter.body.grad_color = LV_COLOR_PURPLE;         /*Dark blue*/
        lv_lmeter_set_range(pm25_lmeter, 75, 150);
        lv_lmeter_set_value(pm25_lmeter, u16Pm25Getter());
    }
    else
    {
        style_pm25_lmeter.text.color = LV_COLOR_PURPLE;
        style_pm25_lmeter.line.color = LV_COLOR_PURPLE;
        style_pm25_lmeter.body.main_color = LV_COLOR_BLACK;         /*Light blue*/
        style_pm25_lmeter.body.grad_color = LV_COLOR_BLACK;         /*Dark blue*/
        lv_lmeter_set_range(pm25_lmeter, 150, 200);
        lv_lmeter_set_value(pm25_lmeter, u16Pm25Getter());
    }
}

void vFreshSensorDispData(void)
{
    if(mainScr != NULL && lv_scr_act() == mainScr)
    {
        char txt[10];
        lv_lmeter_set_value(inDoorH_lmeter, u16IndoorH_Getter() / 10);
        sprintf(txt, "%.1f%%\n \u6e7f\u5ea6", (float)u16IndoorH_Getter() / 10.0);
        lv_label_set_text(inDoorH_Label, txt);
        lv_obj_align(inDoorH_Label, NULL, LV_ALIGN_CENTER, 0, 10);
        //
        lv_lmeter_set_value(inDoorT_lmeter, s16IndoorT_Getter() / 10);
        sprintf(txt, "%.1f\u2103\n \u6e29\u5ea6", (float)s16IndoorT_Getter() / 10.0);
        lv_label_set_text(inDoorT_Label, txt);
        lv_obj_align(inDoorT_Label, NULL, LV_ALIGN_CENTER, 0, 10);
        //
        sprintf(txt, "%d", u16Pm25Getter());
        lv_label_set_text(pm25_Label, txt);

        vModifyPm25Style();
        lv_obj_align(pm25_Label, NULL, LV_ALIGN_CENTER, 0, 0);
    }
}

static lv_res_t mainBtn_click_action(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);

    /* The button is released.
     * Make something here */
    if(id == MAIN_TIME_BUTTON_NUM)
    {
        return timeBtn_click_action();
    }

    if(id == MAIN_SET_BUTTON_NUM)
    {
        return setBtn_click_action();
    }
    else
    {
        return LV_RES_OK;
    }
}

const char weekTableTxt[7][10] = {"\u661f\u671f\u4e00", "\u661f\u671f\4e8c", "\u661f\u671f\u4e09", "\u661f\u671f\u56db", "\u661f\u671f\u4e94", "\u661f\u671f\u516d", "\u661f\u671f\u5929" };

void vFreshTime(lv_obj_t * timeLabel)
{
    if(timeLabel != NULL)
    {
        char txt[30];
        volatile struct Calendar * pCalendar = getter_pCalendar();
        uint8_t  u8Week = RTC_Get_Week(pCalendar->w_year, pCalendar->w_month, pCalendar->w_date);

        if(pCalendar->min < 10)
        {
            sprintf(txt, "%d-%d-%d %d:%d%d %s", pCalendar->w_year, pCalendar->w_month, pCalendar->w_date, pCalendar->hour, 0, pCalendar->min, weekTableTxt[u8Week - 1]);
        }
        else
        {
            sprintf(txt, "%d-%d-%d %d:%d %s", pCalendar->w_year, pCalendar->w_month, pCalendar->w_date, pCalendar->hour, pCalendar->min, weekTableTxt[u8Week - 1]) ;
        }

        lv_label_set_text(timeLabel, txt);
    }
}

static lv_res_t timeBtn_click_action(void)
{
    vInitAdjustTimeDisp();
    vDeleteMainDisp();
    return LV_RES_INV;
}

static lv_res_t setBtn_click_action(void)
{
    vInitSetDisp();
    vDeleteMainDisp();
    return LV_RES_INV;
}

static lv_res_t swAction(lv_obj_t * sw)
{
    uint8_t id = lv_obj_get_free_num(sw);
    if(MAIN_AUTO_SW_NUM==id)
    {
        if(false==lv_sw_get_state(autoSw))
        {
            vFanModeSetter(FAN_MANUAL);
        }
        else
        {
            vFanModeSetter(FAN_AUTO);
        }
    }
    return LV_RES_OK;
}

