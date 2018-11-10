#ifndef  __USR_RTC_H__
#define __USR_RTC_H__

#include "gd32f20x.h"

#define RTC_BKP_FLG 0xA5A5

struct Calendar
{
	uint8_t  hour;
	uint8_t min;
	uint8_t sec;			
	//公历日月年周
	uint16_t w_year;
	uint8_t  w_month;
	uint8_t  w_date;
	uint8_t  week;		 
};

void vInit_RTC(void);
uint8_t vSet_RTC_Time(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);
uint8_t Is_Leap_Year(uint16_t year);
void vSetTimeUnixForm(uint32_t   seccount);
uint8_t RTC_Get(void);
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);
volatile struct Calendar * getter_pCalendar(void);

#endif

