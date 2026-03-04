#ifndef __USART2_H
#define __USART2_H

#include "stm32f10x.h"                  // Device header
#include "usart.h"


extern uint8_t Usart2_RxPacket[6];				//定义接收数据包数组
extern uint8_t Usart2_RxFlag;

void USART2_Config(void);
//uint8_t Usart2_GetRxFlag(void);


#endif


