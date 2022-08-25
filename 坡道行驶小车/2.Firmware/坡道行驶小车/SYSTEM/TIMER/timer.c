#include "timer.h"
#include "led.h"
#include "encode_motor.h"
#include "PID.h"
#include "led.h"
#include "usart.h"
#include "lcd.h"
#include "lcd_init.h" 	

//OLEDˢ����ʾ
void OLED_update()
{
			LCD_ShowIntNum(26,0,(int)V_front_left,3,RED,WHITE,16);
			LCD_ShowIntNum(82,0,(int)V_front_right,3,RED,WHITE,16);
			LCD_ShowIntNum(26,16,(int)V_behind_left,3,RED,WHITE,16);
			LCD_ShowIntNum(82,16,(int)V_behind_right,3,RED,WHITE,16);	
			LCD_ShowIntNum(64,32,(int)PID_OUT,3,RED,WHITE,16);
			LCD_ShowIntNum(64,48,(int)PID_OUT1,3,RED,WHITE,16);
			LCD_ShowIntNum(64,64,(int)PID_OUT2,3,RED,WHITE,16);
			LCD_ShowIntNum(64,80,(int)PID_OUT3,3,RED,WHITE,16);
}	

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM1_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//��ʱ��1�����ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx����						 
}

u16 time_cnt;
//70ms����һ���ٶ�
void TIM1_UP_IRQHandler(void) {
		if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {		//���ָ�����ж��Ƿ���
				TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  			//����жϴ�����λ		
				time_cnt++;
				if(time_cnt >= 7) {
						V_front_left = pause_cnt_front_left * 4.35;	
						V_front_right = pause_cnt_front_right * 4.35;	
						V_behind_left = pause_cnt_behind_left * 4.35;	
						V_behind_right = pause_cnt_behind_right * 4.35;
						//PID�����ٶ�
						PID_calc(V_front_left,V_front_right,V_behind_left,V_behind_right);
						My_CarGo(PID_OUT,PID_OUT1,PID_OUT2,PID_OUT3);
			
						pause_cnt_front_left = 0;
						pause_cnt_front_right = 0;
						pause_cnt_behind_left = 0;
						pause_cnt_behind_right = 0;
			
						time_cnt = 0;
						OLED_update();
				}
		}
}



