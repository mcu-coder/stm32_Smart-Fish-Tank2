# include "Modules.h"


/**
  * @brief  传感器数据扫描
  * @param  无
  * @retval 无
  */

void SensorScan(void)
{	
	
	sensorData.waterlevel = WATER_GetData();
	if(TS_GetData()<= 500)
	{
		sensorData.turb = 0;
	}
	else 
	{
		sensorData.turb = TS_GetData()-500;
	}
	
	uint16_t temp;
	
	for(uint8_t i=0;i<3;i++)
	{
		temp = DS18B20_Get_Temp();
		if(temp !=85)
			{break;}
		delay_ms (500);
	}
	if(temp==85)
		{
			sensorData.temp =0;
		}
	else
		{
			sensorData.temp  = DS18B20_Get_Temp();//读取ds18b20温度数值
		} 
	
	
}


