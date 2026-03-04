#ifndef __WATER_H
#define	__WATER_H
#include "stm32f10x.h"
#include "adcx.h"
#include "delay.h"
#include "math.h"
 

#define WATER_READ_TIMES	10  //WATER传感器ADC循环读取次数

//模式选择	
//模拟AO:	1
//数字DO:	0
#define	MODE1 	1

/***************根据自己需求更改****************/
// WATER GPIO宏定义
#if MODE1
#define		WATER_AO_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	WATER_AO_GPIO_PORT							GPIOA
#define 	WATER_AO_GPIO_PIN								GPIO_Pin_0
#define   ADC_CHANNEL0               			ADC_Channel_0	// ADC 通道宏定义
 
#else
#define		WATER_DO_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	WATER_DO_GPIO_PORT							GPIOA
#define 	WATER_DO_GPIO_PIN								GPIO_Pin_5			

#endif
/*********************END**********************/


void WATER_Init(void);
uint16_t WATER_GetData(void);

#endif /* __WATER_H */

