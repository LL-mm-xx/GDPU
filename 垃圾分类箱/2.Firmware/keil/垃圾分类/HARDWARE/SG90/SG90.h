#ifndef __SG90_H
#define __SG90_H

#include "sys.h"
//TIM3的通道1和2
//void TIM3_Int_Init(u16 arr,u16 psd);

void SG90_1_TIM3_PWM_Init(u16 arr,u16 psd);
void SG90_2_TIM3_PWM_Init(u16 arr,u16 psd);

//void SG90_Init(float angle);

#endif
