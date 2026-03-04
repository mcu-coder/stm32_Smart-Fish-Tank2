#include "stm32f10x.h"
#include "led.h"
#include "beep.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "delay.h"
#include "ds18b20.h"
#include "oled.h"
#include "key.h"
#include "Modules.h"
#include "TIM2.h"
#include "TIM3.h"
#include "adcx.h"
#include "flash.h"
#include "JDQ.h"
#include "ts.h"
#include "Modules.h"
#include "SERVO.h"
#include "WATER.h"
#include "stm32f10x_iwdg.h"
#include "gizwits_product.h"
#include "gizwits_protocol.h"
 



SensorModules sensorData;								//声明传感器数据结构体变量
SensorThresholdValue Sensorthreshold;		//声明传感器阈值结构体变量
DriveModules driveData;									//声明驱动器状态结构体变量

int mode = 0;	//系统模式  1自动  2手动  3设置

//系统静态变量
static uint8_t count_a = 1;  //自动模式按键数
 uint8_t count_m = 1;  //手动模式按键数
static uint8_t count_s = 1;	 //设置模式按键数

WaterChange_State g_WaterChangState = WATER_CHANGE_IDLE ;	// 全局换水状态

/**
  * @brief  显示菜单内容
  * @param  无
  * @retval 无
  */


/**
  * @brief  显示菜单1的固定内容
  * @param  无
  * @retval 无
  */
void OLED_autoPage1(void)		//自动模式菜单第一页
{
	//显示“温度：  ”
	OLED_ShowChinese(0,0,0,16,1); 	//温
	OLED_ShowChinese(16,0,2,16,1);	//度
	OLED_ShowChar(64,0,':',16,1);
	
	//显示“水位:   ”
	OLED_ShowChinese(0,16,22,16,1);//水
	OLED_ShowChinese(16,16,4,16,1);//位
	OLED_ShowChar(64,16,':',16,1);
	
	//显示“浊度：  ”
	OLED_ShowChinese(0,32,6,16,1);	//浊
	OLED_ShowChinese(16,32,2,16,1);	//度
	OLED_ShowChar(64,32,':',16,1);
	
	//显示“喂食时间： ”
	OLED_ShowChinese(0,48,12,16,1); 	//喂
	OLED_ShowChinese(16,48,13,16,1);	//食
	OLED_ShowChinese(32,48,14,16,1);	//时
	OLED_ShowChinese(48,48,15,16,1);	//间
	OLED_ShowChar(64,48,':',16,1);
	
	OLED_Refresh();
	
}


void SensorDataDisplay1(void)		//传感器数据显示第一页
{
	//显示温度数据
	OLED_ShowNum(90,0,sensorData.temp,2,16,1);
	//显示水位数据
	OLED_ShowNum(90,16,sensorData.waterlevel,4,16,1);
	//显示浊度数据
	OLED_ShowNum(90, 32, sensorData.turb,4,16,1);	
	//显示喂食时间数据
	OLED_ShowNum(90,48,Sensorthreshold.feedTime ,2,16,1);

}



/**
  * @brief  显示手动模式设置界面1
  * @param  无
  * @retval 无
  */
void OLED_manualPage1(void)
{
	//显示“灯光：”
	OLED_ShowChinese(16,0,18,16,1);	
	OLED_ShowChinese(32,0,19,16,1);	
	OLED_ShowChar(64,0,':',16,1);

	//显示“风扇：”
	OLED_ShowChinese(16,16,20,16,1);	
	OLED_ShowChinese(32,16,21,16,1);	
	OLED_ShowChar(64,16,':',16,1);
	
	//显示“喂食：”
	OLED_ShowChinese(16,32,12,16,1);	
	OLED_ShowChinese(32,32,13,16,1);	
	OLED_ShowChar(64,32,':',16,1);
	
	//显示“加热片：”
	OLED_ShowChinese(16,48,30,16,1);
	OLED_ShowChinese(32,48,35,16,1);	
	OLED_ShowChinese(48,48,31,16,1);	
	OLED_ShowChar(64,48,':',16,1);
}


/**
  * @brief  显示手动模式设置界面２
  * @param  无
  * @retval 无
  */
void OLED_manualPage2(void)
{
	//显示“抽水：”	
	OLED_ShowChinese(16,0,7,16,1);	
	OLED_ShowChinese(32,0,22,16,1);		
	OLED_ShowChar(64,0,':',16,1);

	//显示“放水：”
	OLED_ShowChinese(16,16,1,16,1);	
	OLED_ShowChinese(32,16,22,16,1);		
	OLED_ShowChar(64,16,':',16,1);
	
}



/**
  * @brief  显示手动模式设置参数界面1
  * @param  无
  * @retval 无
  */
void ManualSettingsDisplay1(void)
{
	if(driveData.LED_Flag)
	{
		OLED_ShowChinese(96,0,24,16,1); 	//开
	}
	else
	{
		OLED_ShowChinese(96,0,26,16,1); 	//关
	}
	
	if(driveData.Fan_Flag)
	{
		OLED_ShowChinese(96,16,24,16,1); 	//开
	}
	else
	{
		OLED_ShowChinese(96,16,26,16,1); 	//关
	}
	
	if(driveData.Servo_Flag)
	{
		OLED_ShowChinese(96,32,24,16,1); 	//开
	}
	else
	{
		OLED_ShowChinese(96,32,26,16,1); 	//关
	}
	
	if(driveData.JRP_Flag)
	{
		OLED_ShowChinese(96,48,24,16,1); 	//开
	}
	else
	{
		OLED_ShowChinese(96,48,26,16,1); 	//关
	}
}

/**
  * @brief  显示手动模式设置参数界面２
  * @param  无
  * @retval 无
  */
void ManualSettingsDisplay2(void)
{
	if(driveData.Bump1_Flag)
	{
		OLED_ShowChinese(96,0,24,16,1); 	//开
	}
	else
	{
		OLED_ShowChinese(96,0,26,16,1); 	//关
	}
	
	if(driveData.Bump2_Flag)
	{
		OLED_ShowChinese(96,16,24,16,1); 	//开
	}
	else
	{
		OLED_ShowChinese(96,16,26,16,1); 	//关
	}
	
}

/**
  * @brief  显示系统阈值设置界面1
  * @param  无
  * @retval 无
  */
void OLED_settingsPage1(void)
{
	//显示“水温上限”
	OLED_ShowChinese(16,0,22,16,1);	
	OLED_ShowChinese(32,0,0,16,1);	
	OLED_ShowChinese(48,0,28,16,1);	
	OLED_ShowChinese(64,0,3,16,1);	
	OLED_ShowChar(80,0,':',16,1);

	//显示“水温下限”
	OLED_ShowChinese(16,16,22,16,1);	
	OLED_ShowChinese(32,16,0,16,1);	
	OLED_ShowChinese(48,16,29,16,1);	
	OLED_ShowChinese(64,16,3,16,1);	
	OLED_ShowChar(80,16,':',16,1);
	
	//显示“水位上限”
	OLED_ShowChinese(16,32,22,16,1);	
	OLED_ShowChinese(32,32,4,16,1);	
	OLED_ShowChinese(48,32,28,16,1);	
	OLED_ShowChinese(64,32,3,16,1);	
	OLED_ShowChar(80,32,':',16,1);
	
	//显示“水位下限”
	OLED_ShowChinese(16,48,22,16,1);	
	OLED_ShowChinese(32,48,4,16,1);	
	OLED_ShowChinese(48,48,29,16,1);	
	OLED_ShowChinese(64,48,3,16,1);	
	OLED_ShowChar(80,48,':',16,1);
	
}

void OLED_settingsPage2(void)
{
	//显示“浊度阈值”
	OLED_ShowChinese(16,0,6,16,1);	
	OLED_ShowChinese(32,0,2,16,1);
	OLED_ShowChinese(48,0,16,16,1);	
	OLED_ShowChinese(64,0,17,16,1);	
	OLED_ShowChar(80,0,':',16,1);

	//显示“喂食时间”
	OLED_ShowChinese(16,16,12,16,1);	
	OLED_ShowChinese(32,16,13,16,1);
	OLED_ShowChinese(48,16,14,16,1);	
	OLED_ShowChinese(64,16,15,16,1);	
	OLED_ShowChar(80,16,':',16,1);
}

void SettingsThresholdDisplay1(void)
{
	//显示水温上限阈值数值
	OLED_ShowNum(90,0, Sensorthreshold.tempValue_MAX , 2,16,1);
	//显示水温下限阈值数值
	OLED_ShowNum(90, 16, Sensorthreshold.tempValue_MIN , 2,16,1);
	//显示水位上限阈值数值
	OLED_ShowNum(90, 32, Sensorthreshold.waterlevel_MAX  , 4,16,1);
	//显示水位下限阈值数值
	OLED_ShowNum(90, 48, Sensorthreshold.waterlevel_MIN , 4,16,1);
}

void SettingsThresholdDisplay2(void)
{
	//显示浊度阈值数值
	OLED_ShowNum(90,0, Sensorthreshold.turbValue , 4,16,1);
	//显示喂食时间
	OLED_ShowNum(90, 16, Sensorthreshold.feedTime , 4,16,1);
		
}
	
/**
  * @brief  记录自动模式界面下按KEY2的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetAuto(void)  
{
	if(KeyNum == KEY_2)
	{
		KeyNum = 0;
		count_a++;
		if (count_a > 2)
		{
			count_a = 1;
		}
		OLED_Clear();
	}
	return count_a;
}

/**
  * @brief  记录手动模式界面下按KEY2的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetManual(void)  
{

	if(KeyNum == KEY_2)
	{
		KeyNum = 0;
		count_m++;
		if (count_m == 5)  		//一共可以控制的外设数量
		{
			OLED_Clear();
		}
		if (count_m > 6)
		{
			OLED_Clear ();
			count_m =1;
		}
	}
	return count_m;
}

/**
  * @brief  记录阈值界面下按KEY2的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetSelection(void)
{
	if(KeyNum == KEY_2)
	{
		KeyNum = 0;
		count_s++;
		if (count_s == 5)
		{
			OLED_Clear();
		}
		else if (count_s > 6)
		{
			OLED_Clear();
			count_s = 1;
		}
	}
	return count_s;
}


/**
  * @brief  显示手动模式界面的选择符号
  * @param  num 为显示的位置
  * @retval 无
  */
void OLED_manualOption(uint8_t num)
{
	switch(num)
	{
		case 1:	
			OLED_ShowChar(0, 0,'>',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		case 2:	
			OLED_ShowChar(0, 0,' ',16,1);
			OLED_ShowChar(0,16,'>',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		case 3:	
			OLED_ShowChar(0, 0,' ',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,'>',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		case 4:	
			OLED_ShowChar(0, 0,' ',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,'>',16,1);
			break;
		case 5:	
			OLED_ShowChar(0, 0,'>',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		case 6:	
			OLED_ShowChar(0, 0,' ',16,1);
			OLED_ShowChar(0,16,'>',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		default: break;
	}
}

/**
  * @brief  显示阈值界面的选择符号
  * @param  num 为显示的位置
  * @retval 无
  */
void OLED_settingsOption(uint8_t num)
{
	switch(num)
	{
		case 1:	
			OLED_ShowChar(0, 0,'>',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		case 2:	
			OLED_ShowChar(0, 0,' ',16,1);
			OLED_ShowChar(0,16,'>',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		case 3:	
			OLED_ShowChar(0, 0,' ',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,'>',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		case 4:	
			OLED_ShowChar(0, 0,' ',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,'>',16,1);
			break;
		case 5:	
			OLED_ShowChar(0, 0,'>',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		case 6:	
			OLED_ShowChar(0, 0,' ',16,1);
			OLED_ShowChar(0,16,'>',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		default: break;
	}
}


/**
  * @brief  自动模式控制函数
  * @param  无
  * @retval 无
  */
void AutoControl(void)
{
 
	
	// 1.基础水位控制(非换水时的常规水位维持) 
	if(sensorData.turb >Sensorthreshold.turbValue && g_WaterChangState == WATER_CHANGE_IDLE )
	{
		driveData.Beep_Flag = 1;	// 开启蜂鸣器
		g_WaterChangState = WATER_CHANGE_DRAIN ;
	}
	
	// 换水状态机执行
	switch (g_WaterChangState )
	{
		case WATER_CHANGE_DRAIN :
			driveData.Bump2_Flag =1 ;
			driveData.Bump1_Flag =0;
		if(sensorData.waterlevel <= Sensorthreshold.waterlevel_MIN )	
		{
			g_WaterChangState = WATER_CHANGE_FILL ;
		}
		break;
		
		case WATER_CHANGE_FILL:
			driveData.Bump2_Flag =0 ;
	        driveData.Bump1_Flag =1 ;
		if(sensorData.waterlevel>=Sensorthreshold.waterlevel_MAX )
		{
			driveData.Bump1_Flag =0;	// 停止抽水
			driveData.Beep_Flag = 0;	// 关闭报警
			g_WaterChangState =WATER_CHANGE_IDLE ;
		}
		break ;
		
		default :
			break ;
	}

	
	if(feed_flag ==1)		// 到达喂食时间进行喂食

	{
		feed_flag =0;
		Servo_SetAngle(10);
		delay_ms(200);
		Servo_SetAngle(160);

	}
}

/**
  * @brief  手动模式控制函数
  * @param  无
  * @retval 无
  */
void ManualControl(uint8_t num)
{
	switch(num)
	{
		case 1:	
			//显示外设开关
			if(KeyNum == KEY_3)
				driveData.LED_Flag = 1;
			if(KeyNum == KEY_4)
				driveData.LED_Flag = 0;
			break;
		case 2:	
			if(KeyNum == KEY_3)
				driveData.Fan_Flag = 1 ;
			if(KeyNum == KEY_4)
				driveData.Fan_Flag = 0;
			break;
	 
	
		default: break;
	}

}

/**
  * @brief  控制函数
  * @param  无
  * @retval 无
  */
void Control_Manager(void)
{
	if(driveData.LED_Flag)
	{
		LED_On();
	}
	else
	{
		LED_Off();
	}
 
}

/**
  * @brief  阈值设置函数
  * @param  无
  * @retval 无
  */
void ThresholdSettings(uint8_t num)
{
	switch (num)
	{
		case 1:
			if (KeyNum == KEY_3)
			{
				KeyNum = 0;
				Sensorthreshold.tempValue_MAX  += 1;
				if (Sensorthreshold.tempValue_MAX > 60)
				{
					Sensorthreshold.tempValue_MAX  = Sensorthreshold.tempValue_MIN +1;
				}
				currentDataPoint.valuetempvalue_MAX = Sensorthreshold.tempValue_MAX; // 同步数据到云端数据点
			}
			 
			break;
			
		case 2:
			if (KeyNum == KEY_3)
			{
				KeyNum = 0;
				Sensorthreshold.tempValue_MIN  += 1;
				if (Sensorthreshold.tempValue_MIN  >= Sensorthreshold.tempValue_MAX )
				{
					Sensorthreshold.tempValue_MIN  = 10;
				}
				currentDataPoint.valuetempvalue_MIN = Sensorthreshold.tempValue_MIN; // 同步数据到云端数据点
			}
			 
			break;
			
		case 3:
			if (KeyNum == KEY_3)
			{
				KeyNum = 0;
				Sensorthreshold.waterlevel_MAX  += 100;
				if (Sensorthreshold.waterlevel_MAX >=4000)
				{
					Sensorthreshold.waterlevel_MAX = Sensorthreshold.waterlevel_MIN  +100;
				}
				currentDataPoint.valuewaterlevelValue_MAX = Sensorthreshold.waterlevel_MAX; // 同步数据到云端数据点
			}
			 
			break;
			 
		default: break;		
	}
}

void ScanGizwitsMode(void)
{
	if(!KEY3)
	{
		delay_ms(20);
		if(!KEY3)
		{
			//显示“热点配网”
	    OLED_ShowChinese(32,16,35,16,1); 	//热
	    OLED_ShowChinese(48,16,36,16,1);	//点                               
	    OLED_ShowChinese(64,16,39,16,1);	//配
	    OLED_ShowChinese(80,16,40,16,1);  	//网
			gizwitsSetMode(WIFI_SOFTAP_MODE);
			OLED_Refresh();
		}
	}
	 
}


/**
  * @brief  flash阈值读取函数
  * @param  无
  * @retval 无
  */
void FLASH_ReadThreshold()
{

	Sensorthreshold.tempValue_MAX = FLASH_R(FLASH_START_ADDR);	//从指定页的地址读FLASH
	Sensorthreshold.tempValue_MIN = FLASH_R(FLASH_START_ADDR+2);//从指定页的地址读FLASH
	Sensorthreshold.waterlevel_MAX = FLASH_R(FLASH_START_ADDR+4);//从指定页的地址读FLASH
	Sensorthreshold.waterlevel_MIN = FLASH_R(FLASH_START_ADDR+6);//从指定页的地址读FLASH
	Sensorthreshold.feedTime = FLASH_R(FLASH_START_ADDR+8);		//从指定页的地址读FLASH
	Sensorthreshold.turbValue = FLASH_R(FLASH_START_ADDR+10);	//从指定页的地址读FLASH
	if(Sensorthreshold.tempValue_MAX > 60 || Sensorthreshold.tempValue_MIN < 10 ||
		Sensorthreshold.waterlevel_MAX  > 2500|| Sensorthreshold.waterlevel_MIN  < 50  || 
		Sensorthreshold.feedTime > 35 || Sensorthreshold.turbValue > 2000)//flash读取乱码重置flash中的数值
	{
		FLASH_W(FLASH_START_ADDR,35,15,1500,200,10,500);
		FLASH_ReadThreshold();
	}
}


int main(void)
{ 
	
	SystemInit();//配置系统时钟为72M	
	delay_init(72);
	
	//传感器初始化

	DS18B20_Init();//ds18b20初始化
	WATER_Init();
	TS_Init();
	 
	Key_Init();//按键初始化
	OLED_Init();
	

	USART1_Config();//串口1初始化
	USART2_Config();//串口2初始化
	USART3_Config();//串口3初始化
	TIM2_Init(72,1000-1);
	TIM3_Int_Init(1000-1,72-1);		//定时1ms中断
		
	OLED_Clear(); 

	OLED_Clear();	
	FLASH_ReadThreshold();
		
  while (1)
  {


		SensorScan();	//获取传感器数据
		 
		switch(mode)
		{
			case AUTO_MODE:
				
					OLED_autoPage1();	//显示主页面1固定信息
					SensorDataDisplay1();	//显示传感器1数据
		
				AutoControl();
				
				/*按键1按下时切换模式*/
				if (KeyNum == KEY_1)   //系统模式mode  1自动  2手动  3设置
				{
					KeyNum = 0;
					mode = MANUAL_MODE;
					count_m = 1;
					OLED_Clear();
				}
				
				 
				
				Control_Manager();
				
				break;
				
			case MANUAL_MODE:
					OLED_manualOption(SetManual());
					ManualControl(SetManual());
				if(SetManual()<=4)
				{	
					OLED_manualPage1();
					ManualSettingsDisplay1();
				}
				else
				{
					OLED_manualPage2();
					ManualSettingsDisplay2();
				}

				
				if (KeyNum == KEY_1)   //系统模式mode  0手动  1自动（默认）
				{
					KeyNum = 0;
					mode = AUTO_MODE;
					count_a = 1;
					OLED_Clear();
				}
				Control_Manager();
				
				break;
				
			case SETTINGS_MODE:

				OLED_settingsOption(SetSelection());	//实现阈值设置页面的选择功能
				ThresholdSettings(SetSelection());	//实现阈值调节功能	
					
				if (SetSelection() <= 4)		
				{				
					OLED_settingsPage1();	//显示阈值设置界面1固定信息
					SettingsThresholdDisplay1();	//显示传感器阈值1数据	
				}
				else	//否则显示设置页面2
				{			
					OLED_settingsPage2();	//显示阈值设置界面2固定信息
					SettingsThresholdDisplay2();	//显示传感器阈值2数据	
				}
					//判断是否退出阈值设置界面
					if (KeyNum == KEY_1)
					{
						KeyNum = 0;
						mode = AUTO_MODE;	//跳转到主界面
						count_a = 1;
						OLED_Clear();	//清屏
						//存储修改的传感器阈值至flash内
						FLASH_W(FLASH_START_ADDR,Sensorthreshold.tempValue_MAX,
						Sensorthreshold.tempValue_MIN ,Sensorthreshold.waterlevel_MAX ,
						Sensorthreshold.waterlevel_MIN,Sensorthreshold.feedTime ,Sensorthreshold.turbValue );
					}
				break;
				default: break;
		}


  }
}
