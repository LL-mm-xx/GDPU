#include "timer.h"
#include "led.h"
#include "usart.h"
#include "control.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!


void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM3_IRQHandler(void)           //�����жϺ��� 50Hz
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
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
                if(Start == 1)      //����Cam�ľ������
                {
                    if(Ball_flag == 0)  //���û���ҵ�����
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
                        IncPID_Pos();                   //��������ƶ�����
                        //TIM_SetCompare1(TIM4,PWM_X);	//�������PID����ֵ
                        if(PWM_X < 1598 && PWM_X > 661) TIM_SetCompare1(TIM4,PWM_X);	//�������PID����ֵ
                        angle_H = (PWM_X - X_MID)*0.0853;   //��ʾ��
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
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}
