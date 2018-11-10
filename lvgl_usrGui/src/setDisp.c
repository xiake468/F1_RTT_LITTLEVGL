#include "setDisp.h"
#include "stdio.h"
#include "mainDisp.h"

static lv_res_t setSetBtn_click_action(void);
static lv_obj_t * setScr = NULL;
static lv_res_t setBtn_click_action(lv_obj_t * btn);

void vInitSetDisp(void)
{
    setScr = lv_obj_create(NULL, NULL);
    lv_scr_load(setScr);
    /*Create a set button*/
    lv_obj_t * setBtn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(setBtn, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, 60, 26);
    lv_obj_set_size(setBtn, 40, 40);
    static lv_style_t style_sym;
    lv_style_copy(&style_sym, &lv_style_plain);
    style_sym.text.color = LV_COLOR_GRAY;
    style_sym.text.font = &lv_font_dejavu_40;       /*The built-in fonts are extended with symbols*/
    lv_obj_t * img_sym = lv_img_create(setBtn, NULL);
    lv_img_set_src(img_sym, SYMBOL_SETTINGS);
    lv_img_set_style(img_sym, &style_sym);
    lv_obj_set_free_num(setBtn, SET_SET_BUTTON_NUM);   /*Set a unique number for the button*/
    lv_btn_set_action(setBtn, LV_BTN_ACTION_CLICK, setBtn_click_action);
}

void vDeleteSetDisp(void)
{
    lv_obj_del(setScr);
}

static lv_res_t setBtn_click_action(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);

    /* The button is released.
     * Make something here */
    if(id == SET_SET_BUTTON_NUM)
    {
        return setSetBtn_click_action();
    }
    else
    {
        return LV_RES_OK;
    }
}

static lv_res_t setSetBtn_click_action(void)
{
    vInitMainDisp();
    vDeleteSetDisp();
    return LV_RES_INV;
}


