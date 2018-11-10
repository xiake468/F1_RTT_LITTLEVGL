#include "touchI2c.h"
#include "usrTimer.h"

void vInitTouchI2c(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    TOUCH_SDA_H;
    TOUCH_SCL_H;
    gpio_init(TOUCH_I2C_SDA_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, TOUCH_I2C_SDA_PIN);
    gpio_init(TOUCH_I2C_SCL_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, TOUCH_I2C_SCL_PIN);
}

void vTouchSdaOut(void)
{
    gpio_init(TOUCH_I2C_SDA_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, TOUCH_I2C_SDA_PIN);
}

void vTouchSdaIn(void)
{
    gpio_init(TOUCH_I2C_SDA_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, TOUCH_I2C_SDA_PIN);
}

void vI2cTouchStart(void)
{
    vTouchSdaOut();
    TOUCH_SCL_H;
    TOUCH_SDA_H;
    vDelay_us(TOUCH_I2C_DELAY);
    TOUCH_SDA_L;
    vDelay_us(TOUCH_I2C_DELAY);
    TOUCH_SCL_L;
}

void vTouchI2cStop(void)
{
    vTouchSdaOut();
    TOUCH_SCL_L;
    TOUCH_SDA_L;
    vDelay_us(TOUCH_I2C_DELAY);
    TOUCH_SCL_H;
    TOUCH_SDA_H;
    vDelay_us(TOUCH_I2C_DELAY);
}

uint8_t u8TouchI2cWaitAck(void)
{
    uint8_t ucErrTime = 0;
    TOUCH_SDA_H;
    vDelay_us(TOUCH_I2C_DELAY);
    vTouchSdaIn();      //SDA设置为输入
    TOUCH_SCL_H;
    vDelay_us(TOUCH_I2C_DELAY);

    while(READ_TOUCH_SDA)
    {
        ucErrTime++;

        if(ucErrTime > 250)
        {
            vTouchI2cStop();
            return 1;
        }

        vDelay_us(1);
    }

    vDelay_us(TOUCH_I2C_DELAY);//
    TOUCH_SCL_L;//时钟输出0
    return 0;
}

void vTouchI2cAck(void)
{
    TOUCH_SCL_L;
    vTouchSdaOut();
    TOUCH_SDA_L;
    vDelay_us(TOUCH_I2C_DELAY);
    TOUCH_SCL_H;
    vDelay_us(TOUCH_I2C_DELAY);
    TOUCH_SCL_L;
}
//不产生ACK应答
void vTouchI2cNAck(void)
{
    TOUCH_SCL_L;
    vTouchSdaOut();
    TOUCH_SDA_H;
    vDelay_us(TOUCH_I2C_DELAY);
    TOUCH_SCL_H;
    vDelay_us(TOUCH_I2C_DELAY);
    TOUCH_SCL_L;
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void TouchIIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    vTouchSdaOut();
    TOUCH_SCL_L;//拉低时钟开始数据传输

    for(t = 0; t < 8; t++)
    {
        if((txd & 0x80) >> 7)
        {
            TOUCH_SDA_H;
        }
        else
        {
            TOUCH_SDA_L;
        }

        txd <<= 1;
        vDelay_us(TOUCH_I2C_DELAY);
        TOUCH_SCL_H;
        vDelay_us(TOUCH_I2C_DELAY);
        TOUCH_SCL_L;
        vDelay_us(TOUCH_I2C_DELAY);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t TouchIIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    vTouchSdaIn();//SDA设置为输入

    for(i = 0; i < 8; i++)
    {
        TOUCH_SCL_L;
        vDelay_us(TOUCH_I2C_DELAY);
        TOUCH_SCL_H;
        receive <<= 1;

        if(READ_TOUCH_SDA)receive++;

        vDelay_us(TOUCH_I2C_DELAY);
        vDelay_us(TOUCH_I2C_DELAY);
    }

    if(!ack)
        vTouchI2cNAck();//发送nACK
    else
        vTouchI2cAck(); //发送ACK

    return receive;
}

unsigned char i2c_read_addr_byte(unsigned char device_addr, unsigned char read_addr)
{
    unsigned char dat;
    vI2cTouchStart();
    TouchIIC_Send_Byte(device_addr << 1);
    u8TouchI2cWaitAck();
    TouchIIC_Send_Byte(read_addr);
    u8TouchI2cWaitAck();
    vTouchI2cStop();

    vI2cTouchStart();
    TouchIIC_Send_Byte((device_addr << 1) | 0x01);
    u8TouchI2cWaitAck();
    dat = TouchIIC_Read_Byte(0);
    vTouchI2cStop();
    return(dat);
}

void i2c_read_addr_str(unsigned char device_addr, unsigned char read_addr, unsigned char read_amount, unsigned char *read_buf)
{
//	uchar dat;
    unsigned char i;
    vI2cTouchStart();
    TouchIIC_Send_Byte(device_addr << 1);
    u8TouchI2cWaitAck();
    TouchIIC_Send_Byte(read_addr);
    u8TouchI2cWaitAck();
    vTouchI2cStop();

    vI2cTouchStart();
    TouchIIC_Send_Byte((device_addr << 1) | 0x01);
    u8TouchI2cWaitAck();

    for(i = 0; i < read_amount - 1; i++)
        read_buf[i] = TouchIIC_Read_Byte(1);

    read_buf[i] = TouchIIC_Read_Byte(0);
    vTouchI2cStop();
}

void i2c_write_addr_byte(unsigned char device_addr, unsigned char write_addr, unsigned char write_dat)
{
    vI2cTouchStart();
    TouchIIC_Send_Byte(device_addr << 1);
    u8TouchI2cWaitAck();
    TouchIIC_Send_Byte(write_addr);
    u8TouchI2cWaitAck();
    TouchIIC_Send_Byte(write_dat);
    u8TouchI2cWaitAck();
    vTouchI2cStop();
    vDelay_ms(2);
}

unsigned int i2c_read_addr_int(unsigned char device_addr, unsigned char read_addr)
{
    unsigned char read_buf[2];
    i2c_read_addr_str(device_addr, read_addr, 2, read_buf);
    return (read_buf[0] << 8) | read_buf[1];
}

void i2c_write_addr_str(unsigned char device_addr, unsigned char write_addr, unsigned char write_amount, unsigned char *write_buf)
{
    unsigned char i;
    vI2cTouchStart();
    TouchIIC_Send_Byte(device_addr << 1);
    u8TouchI2cWaitAck();
    TouchIIC_Send_Byte(write_addr);
    u8TouchI2cWaitAck();

    for(i = 0; i < write_amount; i++)
    {
        TouchIIC_Send_Byte(write_buf[i]);
        u8TouchI2cWaitAck();
    }

    vTouchI2cStop();
    vDelay_ms(2);
}

uint16_t FT6336_read_touch1_x(void)
{
    uint8_t read_buf[2];
    i2c_read_addr_str(I2C_ADDR_FT6336, FT6336_ADDR_TOUCH1_X, 2, read_buf);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}

uint16_t FT6336_read_touch1_y(void)
{
    uint8_t read_buf[2];
    i2c_read_addr_str(I2C_ADDR_FT6336, FT6336_ADDR_TOUCH1_Y, 2, read_buf);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}

uint8_t FT6336_read_touch2_event(void)
{
    return i2c_read_addr_byte(I2C_ADDR_FT6336, FT6336_ADDR_TOUCH2_EVENT) >> 4;
}

uint8_t FT6336_read_touch2_id(void)
{
    return i2c_read_addr_byte(I2C_ADDR_FT6336, FT6336_ADDR_TOUCH2_ID) >> 4;
}

uint16_t FT6336_read_touch2_x(void)
{
    uint8_t read_buf[2];
    i2c_read_addr_str(I2C_ADDR_FT6336, FT6336_ADDR_TOUCH2_X, 2, read_buf);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}

uint16_t FT6336_read_touch2_y(void)
{
    uint8_t read_buf[2];
    i2c_read_addr_str(I2C_ADDR_FT6336, FT6336_ADDR_TOUCH2_Y, 2, read_buf);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}


