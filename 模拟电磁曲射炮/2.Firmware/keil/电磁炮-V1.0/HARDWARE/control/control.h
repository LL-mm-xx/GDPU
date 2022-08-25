#ifndef _CONTROL_H
#define _CONTROL_H

#include "sys.h"

void Shoot_Control(void);    //控制发射
int Calculate(u16 distance); //输入距离计算仰角
void Turn_Angle(float Angle,u8 Servo); //控制转向
void USART2_Rx_Task(void);		//接收openmv数据
void IncPID_Pos(void);				//pid

#endif

