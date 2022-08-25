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

//Num：开关，1为充电开关，2为放电开关
//state：开关状态，0为关，1为开
//IN1为1路，PA11,充电开关；IN2为2路，PA12,放电开关
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

