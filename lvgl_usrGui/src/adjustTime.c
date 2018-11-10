#include "adjustTime.h"
#include "userRTC.h"
#include "mainDisp.h"

static lv_obj_t* adjustTimeScr = NULL;
static lv_obj_t* timeYearShiRoller = NULL;
static lv_obj_t* timeYearGeRoller = NULL;
static lv_obj_t* timeYueRoller = NULL;
static lv_obj_t* timeRiRoller = NULL;
static lv_obj_t* timeShiRoller = NULL;
static lv_obj_t* timeFenRoller = NULL;
static lv_obj_t * timeLabel =NULL;
static lv_res_t adjustTimeBtn_click_action(lv_obj_t * btn);
static lv_res_t exitTimeBtn_click_action(void);
static lv_res_t confirmTimeBtn_click_action(void);

void vInitAdjustTimeDisp(void)
{
    adjustTimeScr = lv_obj_create(NULL, NULL);
    volatile struct Calendar * pCalendar = getter_pCalendar();
    lv_scr_load(adjustTimeScr);
    /*Create a roller and apply the new styles*/
    static lv_style_t bg_style;
    lv_style_copy(&bg_style, &lv_style_pretty);
    bg_style.body.main_color = LV_COLOR_WHITE;
    bg_style.body.grad_color = LV_COLOR_HEX3(0xddd);
    bg_style.body.border.width = 0;
    bg_style.text.line_space = 20;
    bg_style.text.opa = LV_OPA_40;

    static lv_style_t sel_style;
    lv_style_copy(&sel_style, &lv_style_pretty);
    sel_style.body.empty = 1;
    sel_style.body.radius = LV_RADIUS_CIRCLE;
    sel_style.text.color = LV_COLOR_BLUE;
    //20label
    lv_obj_t* _20YeatLabel = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(_20YeatLabel, "20");
    lv_obj_align(_20YeatLabel, NULL, LV_ALIGN_IN_LEFT_MID, 18, 0);  /*Align to the top*/
    //nian
    timeYearShiRoller = lv_roller_create(lv_scr_act(), NULL);
    lv_roller_set_options(timeYearShiRoller, "0\n"
                          "1\n"
                          "2\n"
                          "3\n"
                          "4\n"
                          "5\n"
                          "6\n"
                          "7\n"
                          "8\n"
                          "9");
    lv_roller_set_style(timeYearShiRoller, LV_ROLLER_STYLE_BG, &bg_style);
    lv_roller_set_selected(timeYearShiRoller, pCalendar->w_year % 100 / 10, false);
    lv_roller_set_style(timeYearShiRoller, LV_ROLLER_STYLE_SEL, &sel_style);
    lv_roller_set_visible_row_count(timeYearShiRoller, 3);
    lv_roller_set_hor_fit(timeYearShiRoller, false);
    lv_obj_set_width(timeYearShiRoller, 30);
    lv_obj_align(timeYearShiRoller, _20YeatLabel, LV_ALIGN_OUT_RIGHT_MID, 1, 0);
    //
    timeYearGeRoller = lv_roller_create(lv_scr_act(), NULL);
    lv_roller_set_options(timeYearGeRoller, "0\n"
                          "1\n"
                          "2\n"
                          "3\n"
                          "4\n"
                          "5\n"
                          "6\n"
                          "7\n"
                          "8\n"
                          "9");
    lv_roller_set_style(timeYearGeRoller, LV_ROLLER_STYLE_BG, &bg_style);
    lv_roller_set_selected(timeYearGeRoller, pCalendar->w_year % 10, false);
    lv_roller_set_style(timeYearGeRoller, LV_ROLLER_STYLE_SEL, &sel_style);
    lv_roller_set_visible_row_count(timeYearGeRoller, 3);
    lv_roller_set_hor_fit(timeYearGeRoller, false);
    lv_obj_set_width(timeYearGeRoller, 30);
    lv_obj_align(timeYearGeRoller, timeYearShiRoller, LV_ALIGN_OUT_RIGHT_MID, 1, 0);
    //yue
    timeYueRoller = lv_roller_create(lv_scr_act(), NULL);
    lv_roller_set_options(timeYueRoller, "1\n"
                          "2\n"
                          "3\n"
                          "4\n"
                          "5\n"
                          "6\n"
                          "7\n"
                          "8\n"
                          "9\n"
                          "10\n"
                          "11\n"
                          "12");
    lv_roller_set_style(timeYueRoller, LV_ROLLER_STYLE_BG, &bg_style);
    lv_roller_set_selected(timeYueRoller, pCalendar->w_month - 1, false);
    lv_roller_set_style(timeYueRoller, LV_ROLLER_STYLE_SEL, &sel_style);
    lv_roller_set_visible_row_count(timeYueRoller, 3);
    lv_roller_set_hor_fit(timeYueRoller, false);
    lv_obj_set_width(timeYueRoller, 40);
    lv_obj_align(timeYueRoller, timeYearGeRoller, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    //ri
    timeRiRoller = lv_roller_create(lv_scr_act(), NULL);
    lv_roller_set_options(timeRiRoller, "1\n"
                          "2\n"
                          "3\n"
                          "4\n"
                          "5\n"
                          "6\n"
                          "7\n"
                          "8\n"
                          "9\n"
                          "10\n"
                          "11\n"
                          "12\n"
                          "13\n"
                          "14\n"
                          "15\n"
                          "16\n"
                          "17\n"
                          "18\n"
                          "19\n"
                          "20\n"
                          "21\n"
                          "22\n"
                          "23\n"
                          "24\n"
                          "25\n"
                          "26\n"
                          "27\n"
                          "28\n"
                          "29\n"
                          "30\n"
                          "31");
    lv_roller_set_style(timeRiRoller, LV_ROLLER_STYLE_BG, &bg_style);
    lv_roller_set_selected(timeRiRoller, pCalendar->w_date - 1, false);
    lv_roller_set_style(timeRiRoller, LV_ROLLER_STYLE_SEL, &sel_style);
    lv_roller_set_visible_row_count(timeRiRoller, 3);
    lv_roller_set_hor_fit(timeRiRoller, false);
    lv_obj_set_width(timeRiRoller, 40);
    lv_obj_align(timeRiRoller, timeYueRoller, LV_ALIGN_OUT_RIGHT_MID, 1, 0);
    //Shi
    timeShiRoller = lv_roller_create(lv_scr_act(), NULL);
    lv_roller_set_options(timeShiRoller, "0\n"
                          "1\n"
                          "2\n"
                          "3\n"
                          "4\n"
                          "5\n"
                          "6\n"
                          "7\n"
                          "8\n"
                          "9\n"
                          "10\n"
                          "11\n"
                          "12\n"
                          "13\n"
                          "14\n"
                          "15\n"
                          "16\n"
                          "17\n"
                          "18\n"
                          "19\n"
                          "20\n"
                          "21\n"
                          "22\n"
                          "23");
    lv_roller_set_style(timeShiRoller, LV_ROLLER_STYLE_BG, &bg_style);
    lv_roller_set_selected(timeShiRoller, pCalendar->hour, false);
    lv_roller_set_style(timeShiRoller, LV_ROLLER_STYLE_SEL, &sel_style);
    lv_roller_set_visible_row_count(timeShiRoller, 3);
    lv_roller_set_hor_fit(timeShiRoller, false);
    lv_obj_set_width(timeShiRoller, 40);
    lv_obj_align(timeShiRoller, timeRiRoller, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    //Fen
    timeFenRoller = lv_roller_create(lv_scr_act(), NULL);
    lv_roller_set_options(timeFenRoller, "0\n"
                          "1\n"
                          "2\n"
                          "3\n"
                          "4\n"
                          "5\n"
                          "6\n"
                          "7\n"
                          "8\n"
                          "9\n"
                          "10\n"
                          "11\n"
                          "12\n"
                          "13\n"
                          "14\n"
                          "15\n"
                          "16\n"
                          "17\n"
                          "18\n"
                          "19\n"
                          "20\n"
                          "21\n"
                          "22\n"
                          "23\n"
                          "24\n"
                          "25\n"
                          "26\n"
                          "27\n"
                          "28\n"
                          "29\n"
                          "30\n"
                          "31\n"
                          "32\n"
                          "33\n"
                          "34\n"
                          "35\n"
                          "36\n"
                          "37\n"
                          "38\n"
                          "39\n"
                          "40\n"
                          "41\n"
                          "42\n"
                          "43\n"
                          "44\n"
                          "45\n"
                          "46\n"
                          "47\n"
                          "48\n"
                          "49\n"
                          "50\n"
                          "51\n"
                          "52\n"
                          "53\n"
                          "54\n"
                          "55\n"
                          "56\n"
                          "57\n"
                          "58\n"
                          "59");
    lv_roller_set_style(timeFenRoller, LV_ROLLER_STYLE_BG, &bg_style);
    lv_roller_set_selected(timeFenRoller, pCalendar->min, false);
    lv_roller_set_style(timeFenRoller, LV_ROLLER_STYLE_SEL, &sel_style);
    lv_roller_set_visible_row_count(timeFenRoller, 3);
    lv_roller_set_hor_fit(timeFenRoller, false);
    lv_obj_set_width(timeFenRoller, 40);
    lv_obj_align(timeFenRoller, timeShiRoller, LV_ALIGN_OUT_RIGHT_MID, 1, 0);
    //exit button
     lv_obj_t * adjustTimeExitBtn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(adjustTimeExitBtn, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, 30, 25);
    lv_obj_set_size(adjustTimeExitBtn, 40, 40);
    static lv_style_t style_exit;
    lv_style_copy(&style_exit, &lv_style_plain);
    style_exit.text.color=LV_COLOR_GRAY;
    style_exit.text.font = &lv_font_dejavu_40;       /*The built-in fonts are extended with symbols*/
    lv_obj_t * img_exit = lv_img_create(adjustTimeExitBtn, NULL);
    lv_img_set_src(img_exit, SYMBOL_CLOSE);   
    lv_img_set_style(img_exit, &style_exit);   
    lv_obj_set_free_num(adjustTimeExitBtn, ADJUSTTIME_EXIT_TIME_BUTTON_NUM);   /*Set a unique number for the button*/
    lv_btn_set_action(adjustTimeExitBtn, LV_BTN_ACTION_CLICK, adjustTimeBtn_click_action);
    //confirm button
    lv_obj_t * adjustTimeConfirmBtn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(adjustTimeConfirmBtn, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 30, 25);
    lv_obj_set_size(adjustTimeConfirmBtn, 40, 40);
    static lv_style_t style_sym;
    lv_style_copy(&style_sym, &lv_style_plain);
    style_sym.text.color=LV_COLOR_WHITE;
    style_sym.text.font = &lv_font_dejavu_40;       /*The built-in fonts are extended with symbols*/
    lv_obj_t * img_sym = lv_img_create(adjustTimeConfirmBtn, NULL);
    lv_img_set_src(img_sym, SYMBOL_OK);   
    lv_img_set_style(img_sym, &style_sym);   
    lv_obj_set_free_num(adjustTimeConfirmBtn, ADJUSTTIME_CONFIRM_TIME_BUTTON_NUM);   /*Set a unique number for the button*/
    lv_btn_set_action(adjustTimeConfirmBtn, LV_BTN_ACTION_CLICK, adjustTimeBtn_click_action);
    //timeHint
    timeLabel= lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_size(timeLabel, 220, 22);
    lv_obj_align(timeLabel, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 10, 1);
    vFreshTime(timeLabel);
}

void vDeleteAdjustTimeDisp(void)
{
    lv_obj_del(adjustTimeScr);
}

static lv_res_t adjustTimeBtn_click_action(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);

    /* The button is released.
     * Make something here */
    if(id == ADJUSTTIME_EXIT_TIME_BUTTON_NUM)
    {
        return exitTimeBtn_click_action();
    }
    else if(ADJUSTTIME_CONFIRM_TIME_BUTTON_NUM)
    {
        return confirmTimeBtn_click_action();
    }
    else
    {
        return LV_RES_OK;
    }
}

static lv_res_t exitTimeBtn_click_action(void)
{
    vInitMainDisp();
    vDeleteAdjustTimeDisp();
    return LV_RES_INV;
}

extern const uint8_t mon_table[];
static lv_res_t confirmTimeBtn_click_action(void)
{
    uint16_t syear = 2000 + lv_roller_get_selected(timeYearShiRoller) * 10 + lv_roller_get_selected(timeYearGeRoller);
    uint8_t  smon = 1 + lv_roller_get_selected(timeYueRoller);
    uint8_t sday   = 1 + lv_roller_get_selected(timeRiRoller);

    if(smon==2&&Is_Leap_Year(syear))
    {
        if(sday>29)
        {
            sday=29;
            lv_roller_set_selected(timeRiRoller, sday - 1, false);
        }        
    }
    else
    {
        if(sday > mon_table[smon - 1])
        {
            sday = mon_table[smon - 1];
            lv_roller_set_selected(timeRiRoller, sday - 1, false);
        }
    }

    uint8_t hour   = lv_roller_get_selected(timeShiRoller);
    uint8_t min   = lv_roller_get_selected(timeFenRoller);
    vSet_RTC_Time(syear, smon, sday, hour, min, 0);
    vFreshTime(timeLabel);
    return LV_RES_OK;
}

