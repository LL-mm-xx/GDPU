#ifndef _ADC_H
#define _ADC_H
#include "sys.h"

void ADCx_Init(void);
u16 Get_ADCx_Value(u8 channel,u8 times);
#endif

