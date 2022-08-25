#include "encode_motor.h"
#include "delay.h"
#include "usart.h"
#include "PID.h"

/*******************************************************************************
//编码电机驱动代码
//作者：萝卜明
//CSDN：萝卜明丶
//哔哩哔哩：萝卜明丶
//修改日期:2021年5月10日
//版本：V1.0
********************************************************************************
//V1.0 2021/5/10
*******************************************************************************/

//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_CH1_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE); //使能GPIO外设
	
   //设置该引脚为复用输出功能,输出TIM3 CH1的PWM脉冲波形	GPIOA.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB.4 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.4
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);							 //Pa.4 输出低
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);						 //Pa.5 输出低
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 900;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}

//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_CH2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能GPIO外设
	
   //设置该引脚为复用输出功能,输出TIM3 CH1的PWM脉冲波形	GPIOA.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LED0-->PB.4 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.4
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);						 //Pa.4 输出低
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);						 //Pa.5 输出低
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 900;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}

//TIM4 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void  TIM4_CH1_PWM_Init(u16 arr,u16 psc)	//逻辑输入初始化为1 1
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE); //使能GPIO外设

   //设置该引脚为复用输出功能,输出TIM4 CH1的PWM脉冲波形	GPIOB.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PB.4 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.4
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);						 //Pa.4 输出低
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);						 //Pa.5 输出低
 
   //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 900;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3
}

//TIM4 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void  TIM4_CH2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //使能GPIO外设
	
   //设置该引脚为复用输出功能,输出TIM4 CH1的PWM脉冲波形	GPIOB.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PB.4 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.4
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);						 //Pa.4 输出低
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);						 //Pa.5 输出低
 
   //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 900;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3
}

void Encoder_Init_TIM_Exit0(void)//用于检测编码器A相使用PA0引脚
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PA端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	            //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
 	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//跳变沿触发
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
     
  // 配置中断优先级 													
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Encoder_Init_TIM_Exit1(void)//用于检测编码器A相使用PA1引脚
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PA端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	            //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
 	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上升下降沿触发
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
     
  // 配置中断优先级 													
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Encoder_Init_TIM_Exit2(void)//用于检测编码器A相使用PA2引脚
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PA端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	            //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
	
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2);
 	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//跳变沿触发
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
     
  // 配置中断优先级 													
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Encoder_Init_TIM_Exit3(void)//用于检测编码器A相使用PA3引脚
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PA端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	            //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 

 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);
 	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//跳变沿触发
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
     
  // 配置中断优先级 													
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Encoder_Init(void)
{
	Encoder_Init_TIM_Exit0();
	Encoder_Init_TIM_Exit1();
	Encoder_Init_TIM_Exit2();
	Encoder_Init_TIM_Exit3();
	TIM3_CH1_PWM_Init(999,2);	
	TIM3_CH2_PWM_Init(999,2);	
	TIM4_CH1_PWM_Init(999,2);	
	TIM4_CH2_PWM_Init(999,2);
}

double V_front_left,V_front_right,V_behind_left,V_behind_right;//定时器计算
u16 pause_cnt_front_left,pause_cnt_front_right,pause_cnt_behind_left,pause_cnt_behind_right;//脉冲计数

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
			pause_cnt_front_left++;
			EXTI_ClearITPendingBit(EXTI_Line0);  //清除LINE0上的中断标志位  
}

//外部中断1服务程序
void EXTI1_IRQHandler(void)
{
			pause_cnt_front_right++;
			EXTI_ClearITPendingBit(EXTI_Line1);  //清除LINE1上的中断标志位  
}

//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
			pause_cnt_behind_left++;
			EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE2上的中断标志位  
}

//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
			pause_cnt_behind_right++;
			EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
}

//向前
void CarGo(void)
{
		FRONT_LEFT_F = 1;
		FRONT_LEFT_B = 0;
		BEHIND_LEFT_F = 1;
		BEHIND_LEFT_B = 0;
	
		FRONT_RIGHT_F = 1;
		FRONT_RIGHT_B = 0;
		BEHIND_RIGHT_F = 1;
		BEHIND_RIGHT_B = 0;
	
		delay_ms(10);
}

//后退
void CarBack(void)
{
//	My_CarGo(PID_OUT,PID_OUT1,PID_OUT2,PID_OUT3);
	
	FRONT_LEFT_F  = 0;
	FRONT_LEFT_B  = 1;
	BEHIND_LEFT_F = 0;
	BEHIND_LEFT_B = 1;
	
	FRONT_RIGHT_F  = 0;
	FRONT_RIGHT_B  = 0;
	BEHIND_RIGHT_F = 0;
	BEHIND_RIGHT_B = 0;
	
	delay_ms(10);
}

//向左,驱动右轮
void CarLeft()
{
//	My_CarGo(PID_OUT,PID_OUT1,PID_OUT2,PID_OUT3);

	FRONT_RIGHT_F  = 1;
	FRONT_RIGHT_B  = 0;
	BEHIND_RIGHT_F = 1;
	BEHIND_RIGHT_B = 0;
	
	FRONT_LEFT_F  = 0;
	FRONT_LEFT_B  = 0;
	BEHIND_LEFT_F = 0;
	BEHIND_LEFT_B = 0;

	delay_ms(10);
}

//向右，驱动左轮
void CarRight()
{
//	My_CarGo(PID_OUT,PID_OUT1,PID_OUT2,PID_OUT3);

	FRONT_LEFT_F  = 1;
	FRONT_LEFT_B  = 0;
	BEHIND_LEFT_F = 1;
	BEHIND_LEFT_B = 0;
	
	FRONT_RIGHT_F  = 0;
	FRONT_RIGHT_B  = 0;
	BEHIND_RIGHT_F = 0;
	BEHIND_RIGHT_B = 0;

	delay_ms(10);
}

//停止
void CarStop(void)
{
	TIM_SetCompare1(TIM3 , 1000);
	TIM_SetCompare2(TIM3 , 1000);
	TIM_SetCompare1(TIM4 , 1000);
	TIM_SetCompare2(TIM4 , 1000);
	
	delay_ms(10);
}

//向前
void My_CarGo(int v1,int v2,int v3,int v4)
{
	TIM_SetCompare1(TIM3 , 1000-v1);
	TIM_SetCompare2(TIM3 , 1000-v2);
	TIM_SetCompare1(TIM4 , 1000-v3);
	TIM_SetCompare2(TIM4 , 1000-v4);
}



