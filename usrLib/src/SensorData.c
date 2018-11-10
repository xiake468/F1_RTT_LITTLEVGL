#include "SensorData.h"

static SensorData sensorData;

void vPm25Setter(uint16_t u16TempPm25)
{
    if(u16TempPm25 <= 999)
        sensorData.u16Pm25 = u16TempPm25;
}

uint16_t u16Pm25Getter(void)
{
    return sensorData.u16Pm25;
}

void vPm1Setter(uint16_t u16TempPm1)
{
    if(u16TempPm1 <= 9999)
        sensorData.u16Pm25 = u16TempPm1;
}

uint16_t u16Pm1Getter(void)
{
    return sensorData.u16Pm1;
}

void vPm10Setter(uint16_t u16TempPm10)
{
    if(u16TempPm10 <= 9999)
        sensorData.u16Pm25 = u16TempPm10;
}

uint16_t u16Pm10Getter(void)
{
    return sensorData.u16Pm10;
}

void vPm25_Num_PerL_Setter(uint32_t u32TempPm25_Num_PerL)
{
    sensorData.u32Pm25_Num_PerL = u32TempPm25_Num_PerL;
}

uint32_t u32Pm25_Num_PerL_Getter(void)
{
    return sensorData.u32Pm25_Num_PerL;
}

void vPm1_Num_PerL_Setter(uint32_t u32TempPm1_Num_PerL)
{
    sensorData.u32Pm1_Num_PerL = u32TempPm1_Num_PerL;
}

uint32_t u32Pm1_Num_PerL_Getter(void)
{
    return sensorData.u32Pm1_Num_PerL;
}

void vPm10_Num_PerL_Setter(uint32_t u32TempPm10_Num_PerL)
{
    sensorData.u32Pm10_Num_PerL = u32TempPm10_Num_PerL;
}

uint32_t u32Pm10_Num_PerL_Getter(void)
{
    return sensorData.u32Pm10_Num_PerL;
}

void vHchoSetter(uint16_t u16TempHcho)
{
    if(u16TempHcho <= 9999)
        sensorData.u16Hcho = u16TempHcho;
}

uint16_t u16HchoGetter(void)
{
    return sensorData.u16Hcho;
}

void vTvocSetter(uint16_t u16TempTvoc)
{
    if(u16TempTvoc <= 9999)
        sensorData.u16Tvoc = u16TempTvoc;
}

uint16_t u16TvocGetter(void)
{
    return sensorData.u16Tvoc;
}

void vCo2Setter(uint16_t u16TempCo2)
{
    if(u16TempCo2 <= 9999)
        sensorData.u16Co2 = u16TempCo2;
}

uint16_t u16Co2Getter(void)
{
    return sensorData.u16Co2;
}

void vIndoorH_Setter(uint16_t u16TempIndoorH)
{

    if(u16TempIndoorH < 999)
    {
        sensorData.u16IndoorH = u16TempIndoorH;
    }
}

uint16_t u16IndoorH_Getter(void)
{
    return sensorData.u16IndoorH;
}

void vIndoorT_Setter(int16_t s16TempIndoorT)
{
    if(s16TempIndoorT > -99 && s16TempIndoorT < 999)
        sensorData.s16IndoorT = s16TempIndoorT;
}

int16_t s16IndoorT_Getter(void)
{
    return sensorData.s16IndoorT;
}

