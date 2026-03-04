/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "usart2.h"
#include "Modules.h"
#include "oled.h"
#include "usart.h"
#include "key.h"

#define FLASH_START_ADDR	0x0801f000	//写入的起始地址

void FLASH_W(u32 add,u16 dat,u16 dat2,u16 dat3,u16 dat4,u16 dat5,u16 dat6);

extern SensorModules sensorData;					//声明传感器数据结构体变量
extern SensorThresholdValue Sensorthreshold;		//声明传感器阈值结构体变量
extern DriveModules driveData;						//声明驱动器状态结构体变量
extern int mode;									//系统模式  1自动  2手动  3设置
extern uint8_t count_a;  							//自动模式按键数
extern uint8_t count_m ;  							//手动模式按键数
extern uint8_t count_s;	 							//设置模式按键数

static uint32_t timerMsCount;

/** Current datapoint */
dataPoint_t currentDataPoint;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
    uint8_t i = 0;
    dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
    moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
    protocolTime_t *ptime = (protocolTime_t *)gizdata;
    
#if MODULE_TYPE
    gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
    moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

    if((NULL == info) || (NULL == gizdata))
    {
        return -1;
    }

    for(i=0; i<info->num; i++)
    {
        switch(info->event[i])
        {
        case EVENT_light:
            currentDataPoint.valuelight = dataPointPtr->valuelight;
            GIZWITS_LOG("Evt: EVENT_light %d \n", currentDataPoint.valuelight);
            if(0x01 == currentDataPoint.valuelight)
            {
				OLED_Clear();
				count_m = 1;
				KeyNum = KEY_3;
            }
            else
            {
//				OLED_Clear();
				count_m = 1;
				KeyNum = KEY_4;   
            }
            break;
        case EVENT_fan:
            currentDataPoint.valuefan = dataPointPtr->valuefan;
            GIZWITS_LOG("Evt: EVENT_fan %d \n", currentDataPoint.valuefan);
            if(0x01 == currentDataPoint.valuefan)
            {
				OLED_Clear();
				count_m = 2;
				KeyNum = KEY_3;
            }
            else
            {
//				OLED_Clear();
				count_m = 2;
				KeyNum = KEY_4; 
            }
            break;
        case EVENT_feed:
            currentDataPoint.valuefeed = dataPointPtr->valuefeed;
            GIZWITS_LOG("Evt: EVENT_feed %d \n", currentDataPoint.valuefeed);
            if(0x01 == currentDataPoint.valuefeed)
            {
				OLED_Clear();
				count_m = 3;
				KeyNum = KEY_3;
            }
            else
            {
//				OLED_Clear();
				count_m = 3;
				KeyNum = KEY_4;     
            }
            break;
        case EVENT_JRP:
            currentDataPoint.valueJRP = dataPointPtr->valueJRP;
            GIZWITS_LOG("Evt: EVENT_JRP %d \n", currentDataPoint.valueJRP);
            if(0x01 == currentDataPoint.valueJRP)
            {
				OLED_Clear();
				count_m = 4;
				KeyNum = KEY_3;
            }
            else
            {
//				OLED_Clear();
				count_m = 4;
				KeyNum = KEY_4;   
            }
            break;
        case EVENT_bump1:
            currentDataPoint.valuebump1 = dataPointPtr->valuebump1;
            GIZWITS_LOG("Evt: EVENT_bump1 %d \n", currentDataPoint.valuebump1);
            if(0x01 == currentDataPoint.valuebump1)
            {
				OLED_Clear();
				count_m = 5;
				KeyNum = KEY_3;
            }
            else
            {
//				OLED_Clear();
				count_m = 5;
				KeyNum = KEY_4;     
            }
            break;
        case EVENT_bump2:
            currentDataPoint.valuebump2 = dataPointPtr->valuebump2;
            GIZWITS_LOG("Evt: EVENT_bump2 %d \n", currentDataPoint.valuebump2);
            if(0x01 == currentDataPoint.valuebump2)
            {
				OLED_Clear();
				count_m = 6;
				KeyNum = KEY_3;
            }
            else
            {
//				OLED_Clear();
				count_m = 6;
				KeyNum = KEY_4;   
            }
            break;

        case EVENT_mode:
            currentDataPoint.valuemode = dataPointPtr->valuemode;
            GIZWITS_LOG("Evt: EVENT_mode %d\n", currentDataPoint.valuemode);
            switch(currentDataPoint.valuemode)
            {
            case mode_VALUE0:
					mode =AUTO_MODE;
					OLED_Clear();
					count_m = 1;
                break;
			case mode_VALUE1:
					mode =MANUAL_MODE;
					OLED_Clear();
					count_m = 1;
					driveData.Bump1_Flag=0;
					driveData.Bump2_Flag=0;
					driveData.Fan_Flag=0;
					driveData.JRP_Flag=0;
					driveData.LED_Flag=0;
					driveData.Servo_Flag=0;
                break;
            default:
                break;
            }
            break;

        case EVENT_tempvalue_MAX:
            currentDataPoint.valuetempvalue_MAX = dataPointPtr->valuetempvalue_MAX;
            GIZWITS_LOG("Evt:EVENT_tempvalue_MAX %d\n",currentDataPoint.valuetempvalue_MAX);
            Sensorthreshold.tempValue_MAX =currentDataPoint.valuetempvalue_MAX;
            break;
        case EVENT_tempvalue_MIN:
            currentDataPoint.valuetempvalue_MIN = dataPointPtr->valuetempvalue_MIN;
            GIZWITS_LOG("Evt:EVENT_tempvalue_MIN %d\n",currentDataPoint.valuetempvalue_MIN);
            Sensorthreshold.tempValue_MIN =currentDataPoint.valuetempvalue_MIN;
            break;
        case EVENT_waterlevelValue_MAX:
            currentDataPoint.valuewaterlevelValue_MAX = dataPointPtr->valuewaterlevelValue_MAX;
            GIZWITS_LOG("Evt:EVENT_waterlevelValue_MAX %d\n",currentDataPoint.valuewaterlevelValue_MAX);
            GIZWITS_LOG("Evt:EVENT_waterlevelValue_MAX %d\n",currentDataPoint.valuewaterlevelValue_MAX);
			Sensorthreshold.waterlevel_MAX=currentDataPoint.valuewaterlevelValue_MAX;
            break;
        case EVENT_waterlevelValue_MIN:
            currentDataPoint.valuewaterlevelValue_MIN = dataPointPtr->valuewaterlevelValue_MIN;
            GIZWITS_LOG("Evt:EVENT_waterlevelValue_MIN %d\n",currentDataPoint.valuewaterlevelValue_MIN);
            Sensorthreshold.waterlevel_MIN=currentDataPoint.valuewaterlevelValue_MIN;
            break;
        case EVENT_turbValue:
            currentDataPoint.valueturbValue = dataPointPtr->valueturbValue;
            GIZWITS_LOG("Evt:EVENT_turbValue %d\n",currentDataPoint.valueturbValue);
            Sensorthreshold.turbValue=currentDataPoint.valueturbValue;
            break;
        case EVENT_timeValue:
            currentDataPoint.valuetimeValue = dataPointPtr->valuetimeValue;
            GIZWITS_LOG("Evt:EVENT_timeValue %d\n",currentDataPoint.valuetimeValue);
            Sensorthreshold.feedTime=currentDataPoint.valuetimeValue;
            break;


        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER:
 
            break;
        case WIFI_DISCON_ROUTER:
 
            break;
        case WIFI_CON_M2M:
 
            break;
        case WIFI_DISCON_M2M:
            break;
        case WIFI_RSSI:
            GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
            break;
        case TRANSPARENT_DATA:
            GIZWITS_LOG("TRANSPARENT_DATA \n");
            //user handle , Fetch data from [data] , size is [len]
            break;
        case WIFI_NTP:
            GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
            break;
        case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
#if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",gprsInfoData->Type);
#else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
#endif
        break;
        default:
            break;
        }
    }

    return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
 currentDataPoint.valuelight = driveData.LED_Flag;
    currentDataPoint.valuefan = driveData.Fan_Flag;
    currentDataPoint.valuefeed = driveData.Servo_Flag;
    currentDataPoint.valueJRP = driveData.JRP_Flag;
    currentDataPoint.valuebump1 = driveData.Bump1_Flag;
    currentDataPoint.valuebump2 = driveData.Bump2_Flag;
    currentDataPoint.valuetemp = sensorData.temp;
    currentDataPoint.valuetime = Sensorthreshold.feedTime;
    currentDataPoint.valuetempvalue_MAX = Sensorthreshold.tempValue_MAX;
    currentDataPoint.valuetempvalue_MIN = Sensorthreshold.tempValue_MIN;
    currentDataPoint.valuetimeValue = Sensorthreshold.feedTime;
    currentDataPoint.valuewaterleve = sensorData.waterlevel;
    currentDataPoint.valueturb = sensorData.turb;
    currentDataPoint.valuewaterlevelValue_MAX = Sensorthreshold.waterlevel_MAX;
    currentDataPoint.valuewaterlevelValue_MIN = Sensorthreshold.waterlevel_MIN;
	currentDataPoint.valueturbValue =Sensorthreshold.turbValue;
    currentDataPoint.valuemode = mode;
    
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    currentDataPoint.valuemode = mode_VALUE0;
		driveData.Bump1_Flag=0;
		driveData.Bump2_Flag=0;
		driveData.Fan_Flag=0;
		driveData.JRP_Flag=0;
		driveData.LED_Flag=0;
		driveData.Servo_Flag=0;

}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
		__set_FAULTMASK(1);
		NVIC_SystemReset();
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
  gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  uint8_t value = 0;
  //value = USART_ReceiveData(USART2);//STM32 test demo
  gizPutData(&value, 1);
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
    
    #ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);
    }
    GIZWITS_LOG("\n");
    #endif

    for(i=0; i<len; i++)
    {
		USART_SendData(USART2, buf[i]);//STM32 test demo
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        //USART_SendData(UART, buf[i]);//STM32 test demo
        //Serial port to achieve the function, the buf[i] sent to the module
        if(i >=2 && buf[i] == 0xFF)
        {
			USART_SendData(USART2,0x55);
			while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
          //Serial port to achieve the function, the 0x55 sent to the module
          //USART_SendData(UART, 0x55);//STM32 test demo
        }
    }


    
    return len;
}


