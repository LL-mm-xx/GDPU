#ifndef _CONTROL_H
#define _CONTROL_H

#include "sys.h"

void Shoot_Control(void);    //���Ʒ���
int Calculate(u16 distance); //��������������
void Turn_Angle(float Angle,u8 Servo); //����ת��
void USART2_Rx_Task(void);		//����openmv����
void IncPID_Pos(void);				//pid

#endif

