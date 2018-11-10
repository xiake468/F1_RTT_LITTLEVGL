#ifndef _SENSOR_DATA_H__
#define  _SENSOR_DATA_H__

#include <rthw.h>
#include <rtthread.h>
#include "gd32f20x.h"
#include "main.h"

typedef struct 
{
    uint16_t u16Pm25;
    uint16_t u16Pm1;
    uint16_t u16Pm10;
    uint32_t u32Pm25_Num_PerL;
    uint32_t u32Pm1_Num_PerL;
    uint32_t u32Pm10_Num_PerL;
    uint16_t u16Hcho;
    uint16_t u16Tvoc;
    uint16_t u16Co2;
    uint16_t u16IndoorH;
    int16_t  s16IndoorT;
} SensorData;

void vPm25Setter(uint16_t u16TempPm25);
uint16_t u16Pm25Getter(void);
void vPm1Setter(uint16_t u16TempPm1);
uint16_t u16Pm1Getter(void);
void vPm10Setter(uint16_t u16TempPm10);
uint16_t u16Pm10Getter(void);
void vPm25_Num_PerL_Setter(uint32_t u32TempPm25_Num_PerL);
uint32_t u32Pm25_Num_PerL_Getter(void);
void vPm1_Num_PerL_Setter(uint32_t u32TempPm1_Num_PerL);
uint32_t u32Pm1_Num_PerL_Getter(void);
void vPm10_Num_PerL_Setter(uint32_t u32TempPm10_Num_PerL);
uint32_t u32Pm10_Num_PerL_Getter(void);
void vHchoSetter(uint16_t u16TempHcho);
uint16_t u16HchoGetter(void);
void vTvocSetter(uint16_t u16TempTvoc);
uint16_t u16TvocGetter(void);
void vCo2Setter(uint16_t u16TempCo2);
uint16_t u16Co2Getter(void);
void vIndoorH_Setter(uint16_t u16TempIndoorH);
uint16_t u16IndoorH_Getter(void);
void vIndoorT_Setter(int16_t s16TempIndoorT);
int16_t s16IndoorT_Getter(void);

#endif

