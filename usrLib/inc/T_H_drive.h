#ifndef __T_H_DRIVE_H__
#define __T_H_DRIVE_H__

#include <rthw.h>
#include <rtthread.h>
#include "gd32f20x.h"
#include "main.h"

#define     SHT20_READ                0x80
#define     SHT20_WRITE               0x81

#define     REG_TEMPERATURE           0xE3                
#define     REG_HUMIDITY              0xE5

#define I2C_DELAY 1

#define SI7020_I2C_SCL_PORT GPIOB
#define SI7020_I2C_SCL_PIN  GPIO_PIN_4
#define SI7020_I2C_SDA_PORT GPIOB
#define SI7020_I2C_SDA_PIN  GPIO_PIN_5

#define SCL_H  GPIO_BOP(SI7020_I2C_SCL_PORT)=SI7020_I2C_SCL_PIN
#define SCL_L  GPIO_BC(SI7020_I2C_SCL_PORT)=SI7020_I2C_SCL_PIN
#define SDA_H  GPIO_BOP(SI7020_I2C_SDA_PORT)=SI7020_I2C_SDA_PIN
#define SDA_L  GPIO_BC(SI7020_I2C_SDA_PORT)=SI7020_I2C_SDA_PIN

#define READ_SDA gpio_input_bit_get(SI7020_I2C_SDA_PORT,SI7020_I2C_SDA_PIN)

void vInitSi7020Hw(void);
void vSi7020Read(void);

#endif



