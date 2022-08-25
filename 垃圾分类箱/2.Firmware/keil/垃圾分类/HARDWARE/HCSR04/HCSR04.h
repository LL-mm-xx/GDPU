#ifndef __HCSR04_H
#define __HCSR04_H	 
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
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

void CH_SR04_Init(void);  //������ģ��������ó�ʼ��
float Senor_Using_harmful(void);  //��ຯ��������ֵ��Ϊ����
float Senor_Using_recoverable(void);
float Senor_Using_kitchen(void);
float Senor_Using_other(void);

void NVIC_Config(void);   //�ж�����
		 				    
#endif
