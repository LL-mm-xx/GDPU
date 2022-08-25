#include "sys.h"
#include "usart2.h"	  
//////////////////////////////////////////////////////////////////////////////////	   
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//串口2初始化 
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/5/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明

void Openmv_Receive_Data(int16_t data);
////////////////////////////////////////////////////////////////////////////////// 	

//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率 
void usart2_init(u32 pclk1,u32 bound)
{  	 
  
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟

	//USART1_TX   PA.2 PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //PA2,PA3,复用功能,上拉输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3
	
 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2
  
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode =   USART_Mode_Rx | USART_Mode_Tx;	// 发模式
    USART_Init(USART2, &USART_InitStructure); //初始化串口
    USART_Cmd(USART2, ENABLE);  //使能串口 
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
}


void USART2_IRQHandler(void)			   //串口1全局中断服务函数
{
	u8 temp;
	if( USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET )
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);//清除中断标志
		temp = USART_ReceiveData(USART2);
		Openmv_Receive_Data(temp);//openmv数据处理函数        
    }
}

void Openmv_Receive_Data(int16_t data)//接收Openmv传过来的数据
{
	static u8 state = 0;	
	if(state==0&&data==0x2C)
	{
		state=1;
		RxBuffer1[RxCounter1++]=data;
	}
	else if(state==1&&data==18)
	{
		state=2;
		RxBuffer1[RxCounter1++]=data;
	}
	else if(state==2)
	{
		RxBuffer1[RxCounter1++]=data;
		if(RxCounter1>19||data == 0x5B) state=3;	//the last of char is openmv[19]
	}
	else if(state==3)		//state == 3  检测是否接受到结束标志
	{
        if(RxBuffer1[RxCounter1-1] == 0x5B)
        {
            state = 0;
            RxFlag1 = 1;
            USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
        }else   //wrong thing
        {
            state = 0;
            RxCounter1=0;
        }
	}
    else	//wrong thing
	{
		state = 0;
		RxCounter1=0;
	}
}













