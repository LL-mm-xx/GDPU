#include "timer.h"
#include "led.h"
#include "usart.h"
#include "control.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!


void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM3_IRQHandler(void)           //控制中断函数 50Hz
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
        switch(MODE)
        {
            case MODE1:
                if(Start){
                    angle_V = Calculate(Dis);
                    angle_H = Angle;
                    Turn_Angle(angle_V,1);
                    Turn_Angle(angle_H,2);
                    Shoot_flag = 1;
                }
                else {
                angle_V = 0;angle_H = 0;
                TIM_SetCompare1(TIM4,X_MID);
                TIM_SetCompare2(TIM4,Y_MID);
                  //Turn_Angle(0,1);
                }
                break;
								
            case MODE2:
                if(Start == 1)      //采用Cam的距离测算
                {
                    if(Ball_flag == 0)  //如果没有找到靶子
                    {
                         tt++;
                         if(flag_flip)
												 {
                            if(tt == 50) {angle_H += 5;tt = 0;} 
                            if(angle_H >= 20) flag_flip = 0;
                         }
												 else
												 {
                            if(tt == 50) {angle_H -= 5;tt = 0;} 
                            if(angle_H <= -20) flag_flip = 1;
												 }
												 Turn_Angle(angle_H,2);
                    }
                    else 
                    {
                        IncPID_Pos();                   //计算横向移动距离
                        //TIM_SetCompare1(TIM4,PWM_X);	//输出横向PID计算值
                        if(PWM_X < 1598 && PWM_X > 661) TIM_SetCompare1(TIM4,PWM_X);	//输出横向PID计算值
                        angle_H = (PWM_X - X_MID)*0.0853;   //显示用
                        angle_V = Calculate(Dis_Chao - 30);           
                        if(angle_V > 0)Turn_Angle(angle_V,1);
                        
                        if(fabs(X-Aim_X)<4) t++;
                        if(t == 250) Shoot_flag = 1;
                    }
                }
								break;
								
            default:
                angle_V = 0;angle_H = 0;
                TIM_SetCompare1(TIM4,X_MID);
                TIM_SetCompare2(TIM4,Y_MID);
                break;
        }
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}
