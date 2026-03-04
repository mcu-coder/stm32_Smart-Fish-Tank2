#ifndef	__MODULES_H_
#define __MODULES_H_

#include "stm32f10x.h"                  // Device header
#include "ds18b20.h"
#include "adcx.h"
#include "usart3.h"
#include "ts.h"
#include "WATER.h"

#define KEY_Long1	11

#define KEY_1	1
#define KEY_2	2
#define KEY_3	3
#define KEY_4	4

#define FLASH_START_ADDR	0x0801f000	//写入的起始地址
#define FLASH_FALG    0x1234




typedef enum
{
    AUTO_MODE = 0,    // 自动模式
    MANUAL_MODE,      // 手动模式（值=1）
    SETTINGS_MODE     // 设置模式（值=2）
} MODE_PAGES;  // 枚举类型名，所有文件可直接使用

typedef enum 
{
	WATER_CHANGE_IDLE = 0,	// 空闲状态
	WATER_CHANGE_DRAIN,		// 放水状态
	WATER_CHANGE_FILL		// 抽水状态
}WaterChange_State;

typedef struct
{
	uint16_t turb;		//浊度
	uint8_t temp;		//温度
	uint32_t waterlevel; //水位状态
	
}SensorModules;

typedef struct
{
	uint8_t tempValue_MAX;	//温度上限阈值
	uint8_t tempValue_MIN;	//温度下限阈值
	
	uint32_t waterlevel_MAX;	//水位上限阈值
	uint32_t waterlevel_MIN;	//水位下限阈值
	
	uint16_t turbValue;		// 浊度阈值
	
	uint8_t feedTime;		//喂食时间

	
}SensorThresholdValue;

typedef struct
{
	uint8_t LED_Flag;	//LED灯
	uint8_t Fan_Flag;	//风扇
	uint8_t Bump1_Flag;	//抽水水泵
	uint8_t Bump2_Flag;	//放水水泵
	uint8_t Beep_Flag;	//蜂鸣器
	uint8_t JRP_Flag;	//加热片
	uint8_t Servo_Flag;	//舵机
	
}DriveModules;
 	

extern SensorModules sensorData;			//声明传感器模块的结构体变量
extern SensorThresholdValue Sensorthreshold;	//声明传感器阈值结构体变量
extern DriveModules driveData;				//声明驱动器状态的结构体变量
void SensorScan(void);

#endif
