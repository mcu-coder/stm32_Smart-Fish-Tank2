#ifndef __JDQ_H
#define	__JDQ_H
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"



// 水泵1(抽水)模块 GPIO宏定义
#define	BUMP1_CLK							RCC_APB2Periph_GPIOB

#define BUMP1_GPIO_PIN 				GPIO_Pin_1

#define BUMP1_GPIO_PROT 				GPIOB

#define BUMP1_ON 		GPIO_SetBits(BUMP1_GPIO_PROT,BUMP1_GPIO_PIN)
#define BUMP1_OFF 		GPIO_ResetBits(BUMP1_GPIO_PROT,BUMP1_GPIO_PIN)

// 水泵2(放水)模块 GPIO宏定义
#define	BUMP2_CLK							RCC_APB2Periph_GPIOB

#define BUMP2_GPIO_PIN 				GPIO_Pin_3

#define BUMP2_GPIO_PROT 				GPIOB

#define BUMP2_ON 		GPIO_SetBits(BUMP2_GPIO_PROT,BUMP2_GPIO_PIN)
#define BUMP2_OFF 		GPIO_ResetBits(BUMP2_GPIO_PROT,BUMP2_GPIO_PIN)

// 加热片模块 GPIO宏定义
#define	JRP_CLK							RCC_APB2Periph_GPIOB

#define JRP_GPIO_PIN 				GPIO_Pin_4

#define JRP_GPIO_PORT 				GPIOB

#define JRP_ON 		GPIO_SetBits(JRP_GPIO_PORT,JRP_GPIO_PIN)
#define JRP_OFF 	GPIO_ResetBits(JRP_GPIO_PORT,JRP_GPIO_PIN)

// 风扇模块 GPIO宏定义
#define	FAN_CLK							RCC_APB2Periph_GPIOB

#define FAN_GPIO_PIN 				GPIO_Pin_5

#define FAN_GPIO_PROT 			GPIOB

#define FAN_ON 		GPIO_SetBits(FAN_GPIO_PROT,FAN_GPIO_PIN)
#define FAN_OFF 	GPIO_ResetBits(FAN_GPIO_PROT,FAN_GPIO_PIN)
/*********************END**********************/

void BUMP1_Init(void);
void BUMP2_Init(void);
void JRP_Init(void);
void FAN_Init(void);
#endif



