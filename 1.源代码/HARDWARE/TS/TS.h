#ifndef __TS_H
#define	__TS_H
#include "stm32f10x.h"
#include "adcx.h"
#include "delay.h"
#include "math.h"
 

#define TS_READ_TIMES	10  //TS浊度传感器ADC循环读取次数
#define TS_K 2047.19

/***************根据自己需求更改****************/
// TS GPIO宏定义

#define		TS_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	TS_GPIO_PORT							GPIOA
#define 	TS_GPIO_PIN								GPIO_Pin_1
#define   ADC_CHANNEL1								ADC_Channel_1	// ADC 通道宏定义


/*********************END**********************/


void TS_Init(void);
float TS_GetData(void);

#endif /* __TS_H */

