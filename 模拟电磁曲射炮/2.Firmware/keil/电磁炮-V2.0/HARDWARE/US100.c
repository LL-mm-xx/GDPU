#include "US100.h"
#include "usart.h"
#include "delay.h"
#include "lcd.h"
extern char RxCounter2,RxBuffer2[100];
 
 
char buff[100] = {0};
int tem = 0;
int distance = 0;
u8 Fake_flag = 0;

///通过串口发送读取指令0x55,0x50读取距离与温度信息
////////////////////////////////////
void clear_buff2()
{
	int i;
	for(i=0;i<100;i++)
	RxBuffer1[i]=0;
	RxCounter1=0;
}

void Trig(void)
{
  USART_SendData(USART2,0x55);
	delay_ms(36);
	delay_ms(55);
	USART_SendData(USART2,0x50);
	delay_ms(5);
}

void readTembuff()
{
  if(RxFlag1 == 1)
	{
		distance = RxBuffer1[0]*256+RxBuffer1[1];
    tem = RxBuffer1[2]-45;
    clear_buff2();
		RxFlag1 = 0;RxCounter1 = 0;		
    sprintf((char *)Buf,"dist:%d tem:%d",distance,tem);
    LCD_ShowString(10,10,50,12,16,Buf);
	}
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);      	
}

///通过PA1触发Trigger信号，在PA0上统计Echo低电平时间测距
///////////////////////////////////////////////////////
void UltrasonicWave_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
    //EXTI0 NVIC
		NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
    //EXTI0
		EXTI_InitStructure.EXTI_Line=EXTI_Line0;
		EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd=ENABLE;
		EXTI_Init(&EXTI_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_25MHz;
		GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;;				     //PA0?ECH0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		 //????
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void EXTI0_IRQHandler(void)
{
    static float UltrasonicWave_Distance;
    static u16 sum = 0;
    static u16 Dis_Chao_temp = 0;
    static u16 Dis_Chao_Buf[10]={0};
    static u8 k = 0;
    if(EXTI_GetFlagStatus(EXTI_Line0)==1)
	{
        delay_us(10);	
        Fake_flag = 0;
        TIM_Cmd(TIM2, DISABLE); 
        TIM_SetCounter(TIM2,0);
        TIM_Cmd(TIM2, ENABLE);                                             //开启时钟
        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))                     //等待低电平
        {
            if(TIM_GetCounter(TIM2)>=200) {Fake_flag = 1;break;}           //超时弹出  //200
        }
        TIM_Cmd(TIM2, DISABLE);			                                                    //定时器2失能
        UltrasonicWave_Distance=TIM_GetCounter(TIM2)*34/20.0;								//计算距离&&UltrasonicWave_Distance<150
        if(UltrasonicWave_Distance>0&&Fake_flag == 0)
        {
            printf("distance:%.1f cm",UltrasonicWave_Distance);
            Dis_Chao = (u16) UltrasonicWave_Distance;
            
            Dis_Chao_temp = (u16) UltrasonicWave_Distance;      //采用滤波输出
            sum = sum - Dis_Chao_Buf[k] + Dis_Chao_temp;
            Dis_Chao_Buf[k] = Dis_Chao_temp;
            k++;if(k>9) k = 0;
            Dis_Chao = sum/10;
            
        }
        EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI7线路挂起位
    }
}

void UltrasonicWave_StartMeasure(void)
{
  GPIO_SetBits(GPIOA,GPIO_Pin_1); 	
  delay_us(20);		                     
  GPIO_ResetBits(GPIOA,GPIO_Pin_1);	   
	
}

void UltrasonicWave_Timerx_Init(void)
{   
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM1时钟使能
	TIM_TimeBaseStructure.TIM_Period = 5000-1; //计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(8400-1); // 10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位							 
}




