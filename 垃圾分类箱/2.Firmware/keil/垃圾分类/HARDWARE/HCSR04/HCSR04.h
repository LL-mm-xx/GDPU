#ifndef __HCSR04_H
#define __HCSR04_H	 
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define uint unsigned int
#define TRIG_Send_1 PBout(8)
#define ECHO_Reci_1 PBin(9)
#define TRIG_Send_2 PBout(6)
#define ECHO_Reci_2 PBin(7)
#define TRIG_Send_3 PBout(10)
#define ECHO_Reci_3 PBin(11)
#define TRIG_Send_4 PBout(12)
#define ECHO_Reci_5 PBin(13)

void CH_SR04_Init(void);  //超声波模块相关配置初始化
float Senor_Using_harmful(void);  //测距函数，返回值即为距离
float Senor_Using_recoverable(void);
float Senor_Using_kitchen(void);
float Senor_Using_other(void);

void NVIC_Config(void);   //中断配置
		 				    
#endif
