#ifndef  __TOUCH_I2C_H__
#define __TOUCH_I2C_H__

#include "gd32f20x.h"

#define I2C_ADDR_FT6336 0x38

#define FT6336_ADDR_DEVICE_MODE 	0x00
#define FT6336_ADDR_TD_STATUS 		0x02
#define FT6336_ADDR_TOUCH1_EVENT 	0x03
#define FT6336_ADDR_TOUCH1_ID 		0x05
#define FT6336_ADDR_TOUCH1_X 		0x03
#define FT6336_ADDR_TOUCH1_Y 		0x05

#define FT6336_ADDR_TOUCH2_EVENT 	0x09
#define FT6336_ADDR_TOUCH2_ID 		0x0B
#define FT6336_ADDR_TOUCH2_X 		0x09
#define FT6336_ADDR_TOUCH2_Y 		0x0B

#define FT6336_ADDR_FIRMARE_ID 		0xA6

#define TOUCH_I2C_DELAY 1

#define TOUCH_I2C_SDA_PORT  GPIOB
#define TOUCH_I2C_SDA_PIN   GPIO_PIN_7
#define TOUCH_I2C_SCL_PORT  GPIOB
#define TOUCH_I2C_SCL_PIN   GPIO_PIN_6

#define TOUCH_SCL_H  GPIO_BOP(TOUCH_I2C_SCL_PORT) = TOUCH_I2C_SCL_PIN
#define TOUCH_SDA_H  GPIO_BOP(TOUCH_I2C_SDA_PORT) = TOUCH_I2C_SDA_PIN
#define TOUCH_SCL_L  GPIO_BC(TOUCH_I2C_SCL_PORT) = TOUCH_I2C_SCL_PIN
#define TOUCH_SDA_L  GPIO_BC(TOUCH_I2C_SDA_PORT) = TOUCH_I2C_SDA_PIN

#define READ_TOUCH_SDA  gpio_input_bit_get(TOUCH_I2C_SDA_PORT,TOUCH_I2C_SDA_PIN)

void vInitTouchI2c(void);
uint16_t FT6336_read_touch1_x(void);
uint16_t FT6336_read_touch1_y(void);
uint8_t FT6336_read_touch2_event(void);
uint8_t FT6336_read_touch2_id(void);
uint16_t FT6336_read_touch2_x(void);
uint16_t FT6336_read_touch2_y(void);

#endif

