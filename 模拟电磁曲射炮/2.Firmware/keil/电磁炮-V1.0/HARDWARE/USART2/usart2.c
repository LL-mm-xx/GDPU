#include "sys.h"
#include "usart2.h"	  
//////////////////////////////////////////////////////////////////////////////////	   
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����2��ʼ�� 
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/5/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��

void Openmv_Receive_Data(int16_t data);
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
void usart2_init(u32 pclk1,u32 bound)
{  	 
  
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��

	//USART1_TX   PA.2 PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //PA2,PA3,���ù���,�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù��� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3
	
 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2
  
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode =   USART_Mode_Rx | USART_Mode_Tx;	// ��ģʽ
    USART_Init(USART2, &USART_InitStructure); //��ʼ������
    USART_Cmd(USART2, ENABLE);  //ʹ�ܴ��� 
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
}


void USART2_IRQHandler(void)			   //����1ȫ���жϷ�����
{
	u8 temp;
	if( USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET )
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);//����жϱ�־
		temp = USART_ReceiveData(USART2);
		Openmv_Receive_Data(temp);//openmv���ݴ�����        
    }
}

void Openmv_Receive_Data(int16_t data)//����Openmv������������
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
	else if(state==3)		//state == 3  ����Ƿ���ܵ�������־
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













