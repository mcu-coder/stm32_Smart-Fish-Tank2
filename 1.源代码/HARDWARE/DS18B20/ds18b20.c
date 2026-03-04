#include "ds18b20.h"
#include "delay.h"

/****
*******DS18B20引脚输出配置
*****/
void DS18B20_GPIO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(DS18B20_GPIO_CLK_ENABLE, ENABLE);	 	//使能端口时钟
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;				 					//引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 				//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 				//IO口速度为50MHz
	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);					 			//根据设定参数初始化
}

/****
*******DS18B20引脚输入配置
*****/   
void DS18B20_GPIO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(DS18B20_GPIO_CLK_ENABLE, ENABLE);	 	//使能端口时钟
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;				 					//引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 					//设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 				//IO口速度为50MHz
	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);					 			//根据设定参数初始化
}
/****
*******1.复位操作: 	引脚拉低——延时480-960us——引脚高——延时15-60us
*****/
void DS18B20_RST(void) 																				
{
	DS18B20_GPIO_OUT(); 																				//切换为输出模式
	DS18B20_OUT = 0;																						//拉低
	delay_us(700);
	DS18B20_OUT = 1;																						//拉高
	delay_us(60);
}
/****
*******2.应答操作:	将引脚设置为输入模式——判断引脚低电平时间是否大于60us、小于240us——返回应答结果
*****/
int DS18B20_Check(void) 																			//响应1--失败		响应0----成功
{	
	uint16_t retry=0;
	DS18B20_GPIO_IN();																					//切换到输入模式
	while(DS18B20_IN && retry<10000) 														//引脚一直为高，未被设备主动拉低。提供200us的超时时间
	{
		retry++;
		delay_us(1);
	};
	if(retry>=200) return 1;																		//超时仍为响应（200us）
    else retry=0;
	while(!DS18B20_IN && retry<240)															// 引脚响应则  判断引脚低电平时间是否大于60us、小于240us——返回应答结果
	{
		retry++;
		delay_us(1);
	}
	if(retry>=240)return 1;  																		//应答过时，检查失败     
  return 0; 																								//检验成功，返回0
	
}
/****
*******读0、读1操作
*******将引脚设置为输出模式——引脚拉低——延时2us——引脚拉高——设置为输入模式——延时2us——读取引脚状态——返回读取结果	周期60us
*****/
uint8_t DS18B20_Read_Bit(void) 															
{
	uint8_t data;																								//暂存数据
	DS18B20_GPIO_OUT();																					//切换输出模式
	DS18B20_OUT = 0; 																						//拉低
	delay_us(2);
	DS18B20_OUT = 1;																						//释放
	DS18B20_GPIO_IN();																					//切换输入模式
	delay_us(12);
	if(DS18B20_IN)data =1 ;
	else data = 0;
	delay_us(50);
	//printf("数据：%d\r\n",data);
	return data;

}
/****
*******写1操作 设置引脚为输出模式——引脚拉低——延时2us——引脚拉高——延时大于60us
*****/
void DS18B20_Write_One(void) 
{
  DS18B20_GPIO_OUT();        																//SET PG11 OUTPUT;
	DS18B20_OUT = 0;																						//拉低2us
	delay_us(2);                           
	DS18B20_OUT = 1;
	delay_us(60);
}
/****
*******写0操作 设置引脚为输出模式——引脚拉低——延时60-120us——引脚拉高——延时2us
*****/
void DS18B20_Write_Zero(void)  
{
	DS18B20_GPIO_OUT();        																	//SET PG11 OUTPUT;
	DS18B20_OUT = 0;																						//拉低2us   
	delay_us(60);                           
	DS18B20_OUT = 1;
	delay_us(2);
}
/****
*******读取一个字节8bit
*****/
uint8_t DS18B20_Read_Byte(void)   
{
	u8 i,j,dat;
	dat=0;
	for (i=1;i<=8;i++) 
	{
			j=DS18B20_Read_Bit();
			dat=(j<<7)|(dat>>1);
	}						    
  return dat;
}
/****
*******写一个字节
*****/
void DS18B20_Write_Byte(uint8_t dat)  
 {            
	u8 j;
	u8 testb;
	DS18B20_GPIO_OUT();																				//SET PA0 OUTPUT;
	for (j=1;j<=8;j++) 
	{
		testb=dat&0x01;
		dat=dat>>1;
		if (testb) 
		{
			DS18B20_OUT=0;																	// Write 1
			delay_us(2);                            
			DS18B20_OUT=1;
			delay_us(60);             
		}
		else 
		{
			DS18B20_OUT=0;// Write 0
			delay_us(60);             
			DS18B20_OUT=1;
			delay_us(2);                          
		}
   }
}
 /****
*******启动设备
*****/
void DS18B20_Start(void) 
{                                                                  
   DS18B20_RST();            																//复位
   DS18B20_Check();    		 																	//等待响应
   DS18B20_Write_Byte(0xcc);  															// 发送一条 ROM 指令		0ccH跳过ROM
   DS18B20_Write_Byte(0x44);    														// 发送存储器指令		启动温度转化	
}
//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(DS18B20_GPIO_CLK_ENABLE, ENABLE);	//使能PORTA口时钟 
	
 	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;								//PORTA0 推挽输出
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);

 	GPIO_SetBits(DS18B20_PORT,DS18B20_PIN);    								//输出1

	DS18B20_RST();

	return DS18B20_Check();
}  
// 读取传感器的值
short	DS18B20_Get_Temp(void)
{
	u8 temp;
	uint8_t TH,TL;
	short tem;
	DS18B20_Start();
	DS18B20_RST();
	DS18B20_Check();
	DS18B20_Write_Byte(0xCC); 																// 忽略ROM地址，直接发送命令
	DS18B20_Write_Byte(0xBE); 																// 读取暂存器中9字节数据
	TL = DS18B20_Read_Byte(); 																// 低8位
	TH = DS18B20_Read_Byte(); 																// 低八位
	
	if(TH>7)
	{
			TH=~TH;
			TL=~TL; 
			temp=0;																								//温度为负  
	}else temp=1;																							//温度为正	  	  
	tem=TH; 																									//获得高八位
	tem<<=8;    
	tem+=TL;																									//获得底八位
	tem=(float)tem*0.0625;																			//转换     
	if(temp)return tem; 																			//返回温度值
	else return -tem;    
}


