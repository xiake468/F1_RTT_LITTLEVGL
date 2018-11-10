#ifndef  __DRIVE_LCD_H__
#define __DRIVE_LCD_H__

#include "gd32f20x.h"
#include "lvgl.h"

//Manufacturers offer
#define LCD_HT  400
#define LCD_HPS 30
#define LCD_LPS 30
#define LCD_HPW 20
#define LCD_VT 270
#define LCD_VPS 18
#define LCD_FPS 2
#define LCD_VPW 10
//

#define LCD_WIDTH 320
#define LCD_HIGHT 240

void tli_config(void);
void put_px(int32_t x, int32_t y, lv_color_t  color_p);

#endif
