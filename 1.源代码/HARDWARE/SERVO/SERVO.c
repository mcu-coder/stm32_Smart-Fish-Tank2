 #include "SERVO.h"

volatile uint32_t feed_time_cut = 0;
uint8_t feed_flag = 0;

void Servo_Init(void) {
    //  开启 TIM4 和 GPIOB 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(SERVO_CLK, ENABLE);

    //  配置 PB8 为复用推挽输出（TIM4_CH3）
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = SERVO_GPIO_PIN;    // GPIO_Pin_8
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SERVO_GPIO_PORT, &GPIO_InitStructure);

    //  配置 TIM4 时基单元（20ms周期，72MHz时钟）
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;      // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;      // PSC
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    //  配置 TIM4 通道3为 PWM 模式
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;  // 初始占空比
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);  // 使用 TIM4_CH3
	
	// 	配置TIM4更新中断
	TIM_ITConfig (TIM4,TIM_IT_Update ,ENABLE );		// 开启更新中断
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd  =ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_Init (&NVIC_InitStructure);

    //  启动 TIM4
    TIM_Cmd(TIM4, ENABLE);

}

void PWM_SetCompare3(uint16_t Compare) 
{
    TIM_SetCompare3(TIM4, Compare);  // 修改为 TIM4
}

void Servo_SetAngle(float Angle)
{
    PWM_SetCompare3(Angle / 180 * 2000 + 500);  // 500~2500us 对应 0°~180°
}

void TIM4_IRQHandler()
{
	static uint16_t cut_20ms = 0;
	if(TIM_GetITStatus (TIM4,TIM_IT_Update ) != RESET)
	{
		
		cut_20ms ++;
		// 20ms*5 = 1s
		if(cut_20ms >=50)
		{
			cut_20ms =0;
			feed_time_cut++; //秒数加1
			// 到达设定的时间间隔，置位触发标志
			if(feed_time_cut >= Sensorthreshold.feedTime )
			{
				feed_time_cut = 0;
				feed_flag =1;
			}
		}
		TIM_ClearITPendingBit (TIM4,TIM_IT_Update );
	}
}
