
#ifndef __US100_H_
#define __US100_H_	 
#include "sys.h" 
void Trig(void);
void readTembuff(void);
void UltrasonicWave_Configuration(void);
void EXTI0_IRQHandler(void);
void UltrasonicWave_StartMeasure(void);
void UltrasonicWave_Timerx_Init(void);
#endif

