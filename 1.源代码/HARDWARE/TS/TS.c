#include "ts.h"

 

void TS_Init(void)
{
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd (TS_GPIO_CLK, ENABLE );	// 打开 ADC IO端口时钟
		GPIO_InitStructure.GPIO_Pin = TS_GPIO_PIN;					// 配置 ADC IO 引脚模式
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		// 设置为模拟输入
		
		GPIO_Init(TS_GPIO_PORT, &GPIO_InitStructure);				// 初始化 ADC IO

		ADCx_Init();
	}

}


uint16_t TS_ADC_Read(void)
{
	//设置指定ADC的规则组通道，采样时间
	return ADC_GetValue(ADC_CHANNEL1, ADC_SampleTime_55Cycles5);
}



float TS_GetData(void)
{
	float  tempData = 0;
	float TS_DAT;

	for (uint8_t i = 0; i < TS_READ_TIMES; i++)
	{
		tempData += TS_ADC_Read();
		delay_ms(5);
	}
	tempData /= TS_READ_TIMES;
	
	TS_DAT = (tempData/4096.0)*3.3;
	TS_DAT = -865.68*TS_DAT+TS_K-500;

	if(TS_DAT<35) TS_DAT = 0;
	
	return TS_DAT;

}



