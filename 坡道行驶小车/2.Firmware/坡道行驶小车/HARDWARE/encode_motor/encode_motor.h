#ifndef __ENCODE_MOTER_H
#define	__ENCODE_MOTER_H

#include "stm32f10x.h"

//电机驱动IO定义 
//前左上
#define FRONT_LEFT_F			PBout(0)       
#define FRONT_LEFT_B			PBout(1)      
//前右上
#define FRONT_RIGHT_F			PBout(8)     
#define FRONT_RIGHT_B			PBout(9)      
//后左下
#define BEHIND_LEFT_F			PAout(4)      
#define BEHIND_LEFT_B			PAout(5)      
//后右下
#define BEHIND_RIGHT_F			PCout(13)     
#define BEHIND_RIGHT_B			PBout(5)     

extern double V_front_left,V_front_right,V_behind_left,V_behind_right;//定时器计算
extern u16 pause_cnt_front_left,pause_cnt_front_right,pause_cnt_behind_left,pause_cnt_behind_right;//脉冲计数

void CarGo(void);
void CarBack(void);
void CarLeft(void);
void CarRight(void);
void CarStop(void);
void My_CarGo(int v1,int v2,int v3,int v4);

void Encoder_Init(void);

#endif
