#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "HCSR04.h"
#include "SG90.h"
#include <stdio.h>
#include "bsp_ili9341_lcd.h"
#include "k210_usart.h"

float  harmful_L,recoverable_L,kitchen_L,other_L;

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	ILI9341_Init ();         //LCD 初始化
	uart_init(115200);	 //串口初始化为115200
	K210_USART_Config();
	delay_init();	    	 //延时函数初始化
	CH_SR04_Init();
	SG90_1_TIM3_PWM_Init(199,7199);
	SG90_2_TIM3_PWM_Init(199,7199);
	ILI9341_GramScan ( 5 );
		/********显示变量示例*******/
	LCD_SetFont(&Font16x24);
	LCD_SetColors(BLACK,WHITE);
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
	
	while(1)
	{	
		harmful_L=Senor_Using_harmful();
		delay_ms(100);
		recoverable_L=Senor_Using_recoverable();
		delay_ms(100);
		kitchen_L=Senor_Using_kitchen();
		delay_ms(100);
		other_L=Senor_Using_other();
		delay_ms(100);
		if(harmful_L<10) {delay_ms(500);if(harmful_L<10){ILI9341_DispStringLine_EN(LINE(3),"Food Scrap is full");}}
		else if(harmful_L>15) {LCD_ClearLine(LINE(3));}
		
		if(recoverable_L<10) {delay_ms(500);if(recoverable_L<10){ILI9341_DispStringLine_EN(LINE(5),"Recyclable waste is full");}}
		else if(recoverable_L>15) {LCD_ClearLine(LINE(5));LCD_ClearLine(LINE(6));}
		
		if(kitchen_L<10) {delay_ms(500);if(kitchen_L<10){ILI9341_DispStringLine_EN(LINE(7),"Other waste is full");}}
		else if(kitchen_L>15) {LCD_ClearLine(LINE(7));}

		if(other_L<10) {delay_ms(500);if(other_L<10){ILI9341_DispStringLine_EN(LINE(8),"Harmful waste is    full");}}
		else if(other_L>15) {LCD_ClearLine(LINE(8));LCD_ClearLine(LINE(9));}
	}	 
}
