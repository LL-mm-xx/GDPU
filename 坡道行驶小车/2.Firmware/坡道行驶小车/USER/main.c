/*******************************************************************************
//stm32坡道行驶小车
//作者：萝卜明
//CSDN：萝卜明丶
//哔哩哔哩：萝卜明丶
//修改日期:2021年5月16日
//版本：V1.0								  
********************************************************************************
//V1.0 2021/5/16
//
*******************************************************************************/

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "led.h"
#include "encode_motor.h"
#include "timer.h"
#include "infrared_sensor.h"
#include "PID.h"
#include "lcd.h"
#include "lcd_init.h"

u8 state=0x0f;

//OLED显示
void OLED_Show() 
{
				LCD_ShowString(0,0,"V1:    V2:    ",RED,WHITE,16,0);
				LCD_ShowString(0,16,"V3:    V4:    ",RED,WHITE,16,0);
				LCD_ShowString(0,32,"PIDOUT:   ",RED,WHITE,16,0);
				LCD_ShowString(0,48,"PIDOUT1:   ",RED,WHITE,16,0);
				LCD_ShowString(0,64,"PIDOUT2:   ",RED,WHITE,16,0);
				LCD_ShowString(0,80,"PIDOUT3:   ",RED,WHITE,16,0);
}

int main(void) {
		NVIC_PriorityGroupConfig(2);
		delay_init();	    								
		uart_init(115200);					
		LCD_Init();													//LCD初始化		
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
		OLED_Show();
		Encoder_Init();											//编码电机初始化
		TIM1_Int_Init(100,7200);						//定时器1初始化
		Infrared_Sensor_Init();							//红外循迹模块初始化
		while(1) {
			state = Get_Infrared_Sensor();		//获取红外循迹模块的值
			switch (state) {									//循迹设置
				case 0x00: CarStop(); 	 break;
				case 0x01: CarRight();	 break;
				case 0x03: CarRight();	 break;
				case 0x07: CarRight();	 break;
				case 0x02: CarGo(); 		 break;
				case 0x06: CarGo();			 break; 
				case 0x04: CarGo(); 		 break;
				case 0x0c: CarLeft(); 	 break;
				case 0x08: CarLeft(); 	 break;
				case 0x0e: CarLeft();	   break;
				case 0x0f: 						   break; //识别到杂乱信号设为前进
				case 0x09: 							 break;
				case 0x05:							 break;
				case 0x0a:							 break;
				case 0x0b:							 break;
				default  : CarGo();			 break;					
			}
			state = 0;	
		}	 
}


