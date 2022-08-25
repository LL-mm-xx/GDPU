#include "jidianqi.h"

void jidianqi_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;   //
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);  //
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;  //
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;  
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_12);
}

//Num�����أ�1Ϊ��翪�أ�2Ϊ�ŵ翪��
//state������״̬��0Ϊ�أ�1Ϊ��
//IN1Ϊ1·��PA11,��翪�أ�IN2Ϊ2·��PA12,�ŵ翪��
void jidianqi_state(u8 Num,u8 state)
{
    switch (Num)
    {
        case 1:
            if(state) GPIO_SetBits(GPIOA,GPIO_Pin_11);
            else GPIO_ResetBits(GPIOA,GPIO_Pin_11);
            break;
        case 2:
            if(state) GPIO_SetBits(GPIOA,GPIO_Pin_12);
            else GPIO_ResetBits(GPIOA,GPIO_Pin_12);
            break;
        default:break;
    }
}

