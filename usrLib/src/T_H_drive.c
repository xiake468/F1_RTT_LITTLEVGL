#include "T_H_drive.h"
#include "SensorData.h"
#include "usrTimer.h"

static float l_fTemperature = 0;
static float l_fHumidity = 0;

static void vSi7020Read_T(void);
static void vSi7020Read_H(void);
static void delayH_T_I2c(uint16_t u16Cnt);

void vInitSi7020Hw(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
    SDA_H;
    SCL_H;
    gpio_init(SI7020_I2C_SCL_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SI7020_I2C_SCL_PIN);
    gpio_init(SI7020_I2C_SDA_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SI7020_I2C_SDA_PIN);
    vSi7020Read();
}

void vSdaOut(void)
{
    gpio_init(SI7020_I2C_SDA_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SI7020_I2C_SDA_PIN);
}

void vSdaIn(void)
{
    gpio_init(SI7020_I2C_SDA_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, SI7020_I2C_SDA_PIN);
}

void vI2cStart(void)
{
    vSdaOut();
    SCL_H;
    SDA_H;
    delayH_T_I2c(I2C_DELAY);
    SDA_L;
    delayH_T_I2c(I2C_DELAY);
    SCL_L;
}

void vI2cStop(void)
{
    vSdaOut();
    SCL_H;
    SDA_L;
    delayH_T_I2c(I2C_DELAY);
    SDA_H;
    delayH_T_I2c(I2C_DELAY);
}

uint8_t u8I2cWaitAck(void)
{
    uint8_t ucErrTime = 0;
    vSdaIn();      //SDA设置为输入
    SDA_H;
    delayH_T_I2c(I2C_DELAY);
    SCL_H;
    delayH_T_I2c(I2C_DELAY);

    while(READ_SDA)
    {
        ucErrTime++;

        if(ucErrTime > 10)
        {
            vI2cStop();
            return 1;
        }

        delayH_T_I2c(1);
    }

    SCL_L;//时钟输出0
    return 0;
}

void vI2cAck(void)
{
    SCL_L;
    vSdaOut();
    SDA_L;
    delayH_T_I2c(I2C_DELAY);
    SCL_H;
    delayH_T_I2c(I2C_DELAY);
    SCL_L;
}
//不产生ACK应答
void vI2cNAck(void)
{
    SCL_L;
    vSdaOut();
    SDA_H;
    delayH_T_I2c(I2C_DELAY);
    SCL_H;
    delayH_T_I2c(I2C_DELAY);
    SCL_L;
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    vSdaOut();
    SCL_L;//拉低时钟开始数据传输

    for(t = 0; t < 8; t++)
    {
        if((txd & 0x80) >> 7)
        {
            SDA_H;
        }
        else
        {
            SDA_L;
        }

        txd <<= 1;
        delayH_T_I2c(I2C_DELAY);
        SCL_H;
        delayH_T_I2c(I2C_DELAY);
        SCL_L;
        delayH_T_I2c(I2C_DELAY);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    vSdaIn();//SDA设置为输入

    for(i = 0; i < 8; i++)
    {
        SCL_L;
        delayH_T_I2c(I2C_DELAY);
        SCL_H;
        receive <<= 1;

        if(READ_SDA)receive++;

        delayH_T_I2c(I2C_DELAY);
    }

    if(!ack)
        vI2cNAck();//发送nACK
    else
        vI2cAck(); //发送ACK

    return receive;
}

void vSi7020Read(void)
{
    vSi7020Read_T();
    vDelay_ms(200);
    vSi7020Read_H();
    vDelay_ms(200);
}

static void vSi7020Read_T(void)
{
    uint8_t DataBuf[2];
    uint8_t *pDataBuf = DataBuf;
    vI2cStart();
    IIC_Send_Byte(SHT20_READ);
    u8I2cWaitAck();
    IIC_Send_Byte(REG_TEMPERATURE);
    u8I2cWaitAck();

    vI2cStart();
    IIC_Send_Byte(SHT20_WRITE);
    u8I2cWaitAck();

    vDelay_ms(200);

    *pDataBuf = IIC_Read_Byte(1);/*I2C ReadByte*/
    pDataBuf++;
    *pDataBuf = IIC_Read_Byte(0);/*I2C ReadByte*/
    vI2cStop();
    l_fTemperature = (float)((uint16_t)DataBuf[0] * 256 + DataBuf[1]) * 175.72 / 65536 - 46.85;

    if(l_fTemperature > 99)
        l_fTemperature = 99;

    if(l_fTemperature < -9.9)
        l_fTemperature = -9.9;

    vIndoorT_Setter((int16_t)(l_fTemperature * 10) - 51);
}

static void vSi7020Read_H(void)
{
    uint8_t DataBuf[2];
    uint8_t *pDataBuf = DataBuf;
    vI2cStart();
    IIC_Send_Byte(SHT20_READ);
    u8I2cWaitAck();
    IIC_Send_Byte(REG_HUMIDITY);
    u8I2cWaitAck();

    vI2cStart();
    IIC_Send_Byte(SHT20_WRITE);
    u8I2cWaitAck();

    vDelay_ms(200);

    *pDataBuf = IIC_Read_Byte(1);/*I2C ReadByte*/
    pDataBuf++;
    *pDataBuf = IIC_Read_Byte(0);/*I2C ReadByte*/
    vI2cStop();
    l_fHumidity = (float)((uint16_t)DataBuf[0] * 256 + DataBuf[1]) * 125 / 65536 - 6;

    if(l_fHumidity > 99)
        l_fHumidity = 99;

    if(l_fHumidity < 0)
        l_fHumidity = 0;

    vIndoorH_Setter((uint16_t)(l_fHumidity * 10));
}

 //不放在空闲任务里,修改此函数,实时性更好
static void delayH_T_I2c(uint16_t u16Cnt)
{
    vDelay_us(u16Cnt);
}


