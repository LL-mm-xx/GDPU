#include "timer.h"
#include "led.h"
#include "encode_motor.h"
#include "PID.h"
#include "led.h"
#include "usart.h"
#include "lcd.h"
#include "lcd_init.h" 	

//OLED刷新显示
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

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM1_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//定时器1更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设						 
}

u16 time_cnt;
//70ms计算一次速度
void TIM1_UP_IRQHandler(void) {
		if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {		//检查指定的中断是否发生
				TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  			//清除中断待处理位		
				time_cnt++;
				if(time_cnt >= 7) {
						V_front_left = pause_cnt_front_left * 4.35;	
						V_front_right = pause_cnt_front_right * 4.35;	
						V_behind_left = pause_cnt_behind_left * 4.35;	
						V_behind_right = pause_cnt_behind_right * 4.35;
						//PID控制速度
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



