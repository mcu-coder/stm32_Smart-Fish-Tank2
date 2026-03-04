#ifndef	__BEEP_H
#define	__BEEP_H

#include "stm32f10x.h"                  // Device header

#define BEEP_GPIO_PROT		GPIOC
#define BEEP_GPIO_PIN	  	GPIO_Pin_13
#define BEEP_ON				GPIO_WriteBit(BEEP_GPIO_PROT, BEEP_GPIO_PIN,(BitAction)1);
#define BEEP_OFF			GPIO_WriteBit(BEEP_GPIO_PROT, BEEP_GPIO_PIN,(BitAction)0);



void BEEP_Init(void);
void BEEP_Toggle(void);
void BEEP_Twinkle(void);
#endif
