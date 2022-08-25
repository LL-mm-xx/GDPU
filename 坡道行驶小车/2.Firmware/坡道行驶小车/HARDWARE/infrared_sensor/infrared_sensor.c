#include "infrared_sensor.h"

/*******************************************************************************
//红外传感器驱动代码
//作者：萝卜明
//CSDN：萝卜明丶
//哔哩哔哩：萝卜明丶
//修改日期:2021年5月10日
//版本：V1.0
********************************************************************************
//V1.0 2021/5/10
*******************************************************************************/

//红外传感器初始化
void Behind_Infrared_Sensor_Init()
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; 		     //输入模式
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
}

//红外传感器初始化
void Front_Infrared_Sensor_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //使能PB,PE端口时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//默认为JTAG，需复用成GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; 		     //输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; 		     //输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; 		     //输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
}

//获取传感器状态
u8 Get_Infrared_Sensor(void)
{
		u8 state=0;
		if(PAin(15) == 1)     //高电平表示检测到黑线,指示灯灭
			state|= 0x08;			   //IN4
		if(PAin(12) == 1)
			state|= 0x04;	 			 //IN3
		if(PAin(11) == 1)
			state|= 0x02;				 //IN2
		if(PAin(8) == 1)	
			state|= 0x01;	 		   //IN1
		return state;
}

void Infrared_Sensor_Init(void)
{	
	Behind_Infrared_Sensor_Init();
	Front_Infrared_Sensor_Init();
}
