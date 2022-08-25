#include "Adc.h"
#include "delay.h"
#define RCC_AHB1Periph_GPIOx RCC_AHB1Periph_GPIOA
#define RCC_APB2Periph_ADCx RCC_APB2Periph_ADC1
#define GPIO_Pinx GPIO_Pin_5
#define GPIOx GPIOA
#define ADCx ADC1
#define ADC_Channel_x ADC_Channel_5

void ADCx_Init(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx,ENABLE);  //??????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADCx,ENABLE);  //??ADC??
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pinx;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOx,&GPIO_InitStructure);
	
	ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;  //??????
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	//ADC_InitStructure.ADC_ExternalTrigConv=   //??????????
	ADC_InitStructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion=1;
	ADC_InitStructure.ADC_Resolution=ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;  //????????
	ADC_Init(ADCx,&ADC_InitStructure);
	ADC_Cmd(ADCx,ENABLE);
}

u16 Get_ADCx_Value(u8 channel,u8 times)
{
	u32 temp_val=0;u8 t;
	ADC_RegularChannelConfig(ADCx,channel,1,ADC_SampleTime_480Cycles);
	for(t=0;t<times;t++)
	{
		ADC_SoftwareStartConv(ADCx);
		while(!ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC));
		temp_val+=ADC_GetConversionValue(ADC1);   
		delay_ms(5);
	}	
	return temp_val/times;   
}


