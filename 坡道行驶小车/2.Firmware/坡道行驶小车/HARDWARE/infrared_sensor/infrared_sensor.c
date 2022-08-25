#include "infrared_sensor.h"

/*******************************************************************************
//���⴫������������
//���ߣ��ܲ���
//CSDN���ܲ���ؼ
//�����������ܲ���ؼ
//�޸�����:2021��5��10��
//�汾��V1.0
********************************************************************************
//V1.0 2021/5/10
*******************************************************************************/

//���⴫������ʼ��
void Behind_Infrared_Sensor_Init()
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; 		     //����ģʽ
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
}

//���⴫������ʼ��
void Front_Infrared_Sensor_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//Ĭ��ΪJTAG���踴�ó�GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; 		     //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; 		     //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; 		     //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
}

//��ȡ������״̬
u8 Get_Infrared_Sensor(void)
{
		u8 state=0;
		if(PAin(15) == 1)     //�ߵ�ƽ��ʾ��⵽����,ָʾ����
			state|= 0x08;			   //IN4
		if(PAin(12) == 1)
			state|= 0x04;	 			 //IN3
		if(PAin(11) == 1)
			state|= 0x02;				 //IN2
		if(PAin(8) == 1)	
			state|= 0x01;	 		   //IN1
		return state;
}

void Infrared_Sensor_Init(void)
{	
	Behind_Infrared_Sensor_Init();
	Front_Infrared_Sensor_Init();
}
