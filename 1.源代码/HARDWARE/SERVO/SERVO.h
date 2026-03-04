#ifndef __SERVO_H
#define	__SERVO_H
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "Modules.h"



/***************根据自己需求更改****************/
// SERVO舵机 GPIO宏定义

#define	SERVO_CLK										RCC_APB2Periph_GPIOB

#define SERVO_GPIO_PIN 							GPIO_Pin_8
#define SERVO_GPIO_PORT  						GPIOB

/*********************END**********************/

extern volatile uint32_t feed_time_cut;		//计时计数器
extern uint8_t feed_flag;					//喂食

void Servo_Init(void);
void PWM_SetCompare3(uint16_t Compare);
void Servo_SetAngle(float Angle);

#endif



