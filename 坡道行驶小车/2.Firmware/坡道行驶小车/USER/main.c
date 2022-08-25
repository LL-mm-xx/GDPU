/*******************************************************************************
//stm32�µ���ʻС��
//���ߣ��ܲ���
//CSDN���ܲ���ؼ
//�����������ܲ���ؼ
//�޸�����:2021��5��16��
//�汾��V1.0								  
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

//OLED��ʾ
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
		LCD_Init();													//LCD��ʼ��		
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
		OLED_Show();
		Encoder_Init();											//��������ʼ��
		TIM1_Int_Init(100,7200);						//��ʱ��1��ʼ��
		Infrared_Sensor_Init();							//����ѭ��ģ���ʼ��
		while(1) {
			state = Get_Infrared_Sensor();		//��ȡ����ѭ��ģ���ֵ
			switch (state) {									//ѭ������
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
				case 0x0f: 						   break; //ʶ�������ź���Ϊǰ��
				case 0x09: 							 break;
				case 0x05:							 break;
				case 0x0a:							 break;
				case 0x0b:							 break;
				default  : CarGo();			 break;					
			}
			state = 0;	
		}	 
}


