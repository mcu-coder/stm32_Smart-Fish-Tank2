#include "key.h"
 

//KEY1---短按/长按

uint16_t		key1_cnt;					//CNT计数标志
uint8_t			key1_short_flag;	//短按标志
uint8_t			key1_lock_flag;		//长按-锁标志
uint8_t			key1_long_flag;		//长按-标志


//KEY2---短按/长按

uint16_t	key2_cnt;					//CNT计数标志
uint8_t		key2_short_flag;	//短按标志
uint8_t		key2_lock_flag;		//长按-锁标志
uint8_t		key2_long_flag;		//长按-标志


//KEY3---短按/连按

uint16_t	key3_cnt;					//CNT计数标志
uint16_t	key3_cnt2;				
uint8_t		key3_short_flag;	//短按标志
uint8_t		key3_lock_flag;		//长按-锁标志




//KEY4---短按/连按

uint16_t	key4_cnt;					//CNT计数标志
uint16_t	key4_cnt2;				
uint8_t		key4_short_flag;	//短按标志
uint8_t		key4_lock_flag;		//长按-锁标志


u8 KeyNum = 0;							//按键码
/**
  * @brief  Key初始化
  * @param  无
  * @retval 无
  */
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN | KEY2_GPIO_PIN | KEY3_GPIO_PIN | KEY4_GPIO_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
	
}

/**
  * @brief  Key_scan() 用于定时为1ms的中断中，主程序通过判断KeyNum的值进行按键状态判断,KEY1，KEY2支持短按/长按，KEY3,KEY4 支持短按/连击
  * @param  无
  * @retval 返回触发按键的键值
  */
void Key_scan(void)
{
	
	/*按键松手进行状态判断*/
	if(KEY1) 												   //如果没有按键按下
	{
		key1_lock_flag = 0;					     //清零自锁标志
		key1_cnt = 0; 								   //清零计数标志
		if(key1_short_flag)
		{
			key1_short_flag = 0;			     //清零短按标志位
			KeyNum = KEY1_Short; 					 //赋键值编码 短按键值编码
		}
		if(key1_long_flag)
		{
			key1_long_flag = 0;						  //清零长按标志位
			KeyNum = KEY1_Long; 						//赋键值编码 长按键值编码
		}
	}
	
	/*按键按下*/     										//默认为短按
	else if(!key1_lock_flag)
	{
		key1_cnt++; 											//累计按键消抖延时次数
		if(key1_cnt > KEY_DELAY_TIME)		// KEY_DELAY_TIME--消抖延时
		{
			key1_cnt = 0; 									//清零计数标志
			key1_short_flag = 1;						//短按标志位
			key1_long_flag = 0;							//赋长按标志位
			key1_lock_flag = 1; 						//自锁标志置1 防止多次触发
		}
	}
	
		else
	{
		key1_cnt++;												//累计按键长按功能延时时间
		
		if(key1_cnt > KEY_LONG_TIME	)			//KEY_LONG_TIME--长按阈值
		{
			
			key1_short_flag = 0;						//短按---改长按
			key1_long_flag = 1;							//赋长按标志位
		}
	}

/*-------------------------------------------------------------------------------------------------*/		
	
	/*按键松手进行状态判断*/
	if(KEY2) 														 //如果没有按键按下
	{
		key2_lock_flag = 0; 							 //清零自锁标志
		key2_cnt = 0; 										 //清零计数标志
		if(key2_short_flag)
		{
			key2_short_flag = 0; 						 //清零短按标志位
			KeyNum = KEY2_Short; 						 //赋键值编码 短按键值编码
		}
		if(key2_long_flag)
		{
			key2_long_flag = 0;							 //清零长按标志位
			KeyNum = KEY2_Long;							 //赋键值编码 长按键值编码
		}
	}
	
	/*按键按下*/ 													//默认为短按
	else if(!key2_lock_flag)
	{
		key2_cnt++; 												//累计按键消抖延时次数
		if(key2_cnt > KEY_DELAY_TIME)
		{
			key2_cnt = 0; 										//清零计数标志
			key2_short_flag = 1;
			key2_long_flag = 0;								//赋长按标志位
			key2_lock_flag = 1; 							//自锁标志置1 防止多次触发
		}
	}
		else
	{
		key2_cnt++;													//累计按键长按功能延时时间

		if(key2_cnt > KEY_LONG_TIME)
		{
			key2_short_flag = 0;							//短按---改长按
			key2_long_flag = 1;								//赋长按标志位
		}
	}
	
	
/*-------------------------------------------------------------------------------------------------*/	
	
	
	/*按键松手/按键未按下*/
	if(KEY3) 																   //如果没有按键按下
	{
		key3_lock_flag = 0; 									   //清零自锁标志
		key3_cnt = 0; 												   //清零计数标志
				 
	}
	/*按键按下*/ 															  //默认为短按
	else if(!key3_lock_flag)
	{
		key3_cnt++; 														  //累计按键消抖延时次数
		if(key3_cnt > KEY_DELAY_TIME)
		{
			key3_cnt = 0; 												  //清零计数标志
			KeyNum = KEY3_Short; 										//赋键值编码 短按键值编码
			key3_lock_flag = 1; 									  //自锁标志置1 防止多次触发
		}
	}
																						  
	else if(key3_cnt < KEY_Continue_TIME)				//KEY_Continue_TIME---短按最长停留时间
	{
		key3_cnt++;															  //累计按键连击功能触发时间
	}
																							
	else																				//按键已长时间按下--进行连击阈值判断		
	{

		key3_cnt2++;															//累计按键连击功能延迟时间
		if(key3_cnt2 > KEY_Continue_Trigger_TIME )//KEY_Continue_Trigger_TIME--连击阈值
		{
			key3_cnt2 = 0;													//清零连击延迟时间
			KeyNum = KEY3_Lianji;										//赋连击编码键值
		}
		

	
	}
	
/*-------------------------------------------------------------------------------------------------*/		
	
	
	if(KEY4) 																		   //如果没有按键按下
	{
		key4_lock_flag = 0; 											   //清零自锁标志
		key4_cnt = 0; 														   //清零计数标志	
	}
	else if(!key4_lock_flag)
	{
		key4_cnt++; 															   //累计按键消抖延时次数
		if(key4_cnt > KEY_DELAY_TIME)
		{
			key4_cnt = 0; 													   //清零计数标志
			KeyNum =  KEY4_Short; 										 //赋键值编码 短按键值编码
			key4_lock_flag = 1; 											 //自锁标志置1 防止多次触发
		}
	}
	else if(key4_cnt < KEY_Continue_TIME)
	{
		key4_cnt++;																	  //累计按键连击功能触发时间
	}
	else																						//按键已长时间按下--进行连击阈值判断	
	{	
		key4_cnt2++;																	//累计按键连击功能延迟时间
		if(key4_cnt2 > KEY_Continue_Trigger_TIME)
		{
			key4_cnt2 = 0;														  //清零连击延迟时间
			KeyNum = KEY4_Lianji;												//赋连击编码键值
		}

	
	}
}



















