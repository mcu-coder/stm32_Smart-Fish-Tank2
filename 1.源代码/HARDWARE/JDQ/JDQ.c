#include "JDQ.h"


// 水泵1(抽水)模块
void BUMP1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(BUMP1_CLK, ENABLE ); //配置时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = BUMP1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(BUMP1_GPIO_PROT,&GPIO_InitStructure);

	BUMP1_OFF;
}

// 水泵2(放水)模块
void BUMP2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(BUMP2_CLK, ENABLE ); //配置时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = BUMP2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(BUMP2_GPIO_PROT,&GPIO_InitStructure);

	BUMP2_OFF;
}

// 加热片模块
void JRP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(JRP_CLK, ENABLE ); //配置时钟
	
	GPIO_InitStructure.GPIO_Pin = JRP_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(JRP_GPIO_PORT,&GPIO_InitStructure);

	JRP_OFF;
}

// 风扇模块
void FAN_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(FAN_CLK, ENABLE ); //配置时钟
	
	GPIO_InitStructure.GPIO_Pin = FAN_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(FAN_GPIO_PROT,&GPIO_InitStructure);

	FAN_OFF;
}

