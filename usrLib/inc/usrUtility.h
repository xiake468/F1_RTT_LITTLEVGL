#ifndef _USR_UTILITY_H__
#define  _USR_UTILITY_H__
 
#include <rthw.h>
#include <rtthread.h>
#include "gd32f20x.h"
#include "main.h"

void USART_SendStr(uint32_t USARTx, uint8_t *str, uint16_t str_len);
uint16_t u16CalCrc(uint8_t *pu8Data , uint16_t u16DataLen);
uint16_t u16ConvBtoL(uint8_t* pU8_Ptr);
uint32_t u32ConvBtoL(uint8_t* pU8_Ptr);

#endif




