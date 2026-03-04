#include "beep.h"
#include "delay.h"

void BEEP_Init(void)
{
	//开启GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	//配置LED引脚为模拟输出模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BEEP_GPIO_PROT, &GPIO_InitStructure);
	GPIO_ResetBits(BEEP_GPIO_PROT, BEEP_GPIO_PIN);
}

void BEEP_Toggle(void)
{
	GPIO_WriteBit(BEEP_GPIO_PROT, BEEP_GPIO_PIN, (BitAction)((1-GPIO_ReadOutputDataBit(BEEP_GPIO_PROT, BEEP_GPIO_PIN))));//led电平翻转
}

void BEEP_Twinkle()
{
	BEEP_ON;
	delay_ms(10);
	BEEP_OFF;
}
