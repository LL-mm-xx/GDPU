#include "k210_usart.h"
#include "bsp_ili9341_lcd.h"
#include "SG90.h"
#include "delay.h"
#include "HCSR04.h"

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
uint8_t ucTemp;
uint8_t num_count=0;
uint8_t Recyclable_count=0;
uint8_t Harmful_count=0;
uint8_t Food_count=0;
uint8_t Other_count=0;

char dispBuff[500];
	
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void K210_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);		

  // ���������ɱ�־
	//USART_ClearFlag(USART1, USART_FLAG_TC);     
}

/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}

// �����жϷ�����
void DEBUG_USART_IRQHandler(void)
{
  uint8_t ucTemp;
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(DEBUG_USARTx);
		switch(ucTemp)
		{
			case 0: USART_SendData(DEBUG_USARTx,ucTemp);
							delay_ms(500);TIM_SetCompare2(TIM3,176);delay_ms(500);TIM_SetCompare2(TIM3,181);
			
							num_count++;Recyclable_count++;LCD_ClearLine(LINE(0));
							
							sprintf(dispBuff,"%d Recyclable %d OK!",num_count,Recyclable_count);
			
							ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;
			
			case 1: USART_SendData(DEBUG_USARTx,ucTemp);
							delay_ms(500);TIM_SetCompare2(TIM3,176);delay_ms(500);TIM_SetCompare2(TIM3,181);
			
							num_count++;Recyclable_count++;LCD_ClearLine(LINE(0));
			
							sprintf(dispBuff,"%d Recyclable %d OK!",num_count,Recyclable_count);
					
							ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;
			
			case 2: USART_SendData(DEBUG_USARTx,ucTemp);
							delay_ms(500);TIM_SetCompare1(TIM3,174);delay_ms(500);TIM_SetCompare1(TIM3,177.8);
			
							num_count++;Harmful_count++;LCD_ClearLine(LINE(0));
							
							sprintf(dispBuff,"%d Harmful %d OK!",num_count,Harmful_count);
							
							ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;
			
			case 3: USART_SendData(DEBUG_USARTx,ucTemp);					
							delay_ms(500);TIM_SetCompare1(TIM3,187);delay_ms(500);TIM_SetCompare1(TIM3,177.8);
							
							num_count++;Food_count++;LCD_ClearLine(LINE(0));
							
							sprintf(dispBuff,"%d Food Scrap %d OK!",num_count,Food_count);
							
							ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;
			
			case 4: USART_SendData(DEBUG_USARTx,ucTemp);
							delay_ms(500);TIM_SetCompare1(TIM3,187);delay_ms(500);TIM_SetCompare1(TIM3,177.8);
							
							num_count++;Food_count++;LCD_ClearLine(LINE(0));
							
							sprintf(dispBuff,"%d Food Scrap %d OK!",num_count,Food_count);
							
							ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;
							
			case 5: USART_SendData(DEBUG_USARTx,ucTemp);
							delay_ms(500);TIM_SetCompare1(TIM3,187);delay_ms(500);TIM_SetCompare1(TIM3,177.8);
							
							num_count++;Food_count++;LCD_ClearLine(LINE(0));
							
							sprintf(dispBuff,"%d Food Scrap %d OK!",num_count,Food_count);
							
							ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;
							
			case 6: USART_SendData(DEBUG_USARTx,ucTemp);
							delay_ms(500);TIM_SetCompare1(TIM3,187);delay_ms(500);TIM_SetCompare1(TIM3,177.8);
							
							num_count++;Food_count++;LCD_ClearLine(LINE(0));
							
							sprintf(dispBuff,"%d Food Scrap %d OK!",num_count,Food_count);
							
							ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;						
													
			case 7: USART_SendData(DEBUG_USARTx,ucTemp);
							delay_ms(500);TIM_SetCompare1(TIM3,187);delay_ms(500);TIM_SetCompare1(TIM3,177.8);
							
							num_count++;Food_count++;LCD_ClearLine(LINE(0));
							
							sprintf(dispBuff,"%d Food Scrap %d OK!",num_count,Food_count);
							
							ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;
							
			case 8: USART_SendData(DEBUG_USARTx,ucTemp);
							delay_ms(500);TIM_SetCompare1(TIM3,187);delay_ms(500);TIM_SetCompare1(TIM3,177.8);
							
							num_count++;Food_count++;LCD_ClearLine(LINE(0));
							
							sprintf(dispBuff,"%d Food Scrap %d OK!",num_count,Food_count);
							
							ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;
							
			case 9: USART_SendData(DEBUG_USARTx,ucTemp);
							delay_ms(500);TIM_SetCompare1(TIM3,187);delay_ms(500);TIM_SetCompare1(TIM3,177.8);
							
							num_count++;Food_count++;LCD_ClearLine(LINE(0));
							
							sprintf(dispBuff,"%d Food Scrap %d OK!",num_count,Food_count);
							
							ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;
							
			case 10: USART_SendData(DEBUG_USARTx,ucTemp);
							delay_ms(500);TIM_SetCompare1(TIM3,187);delay_ms(500);TIM_SetCompare1(TIM3,177.8);
							
							num_count++;Food_count++;LCD_ClearLine(LINE(0));
							
							sprintf(dispBuff,"%d Food Scrap %d OK!",num_count,Food_count);
							
							ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;
							
			case 11: USART_SendData(DEBUG_USARTx,ucTemp);
							delay_ms(500);TIM_SetCompare2(TIM3,187);delay_ms(500);TIM_SetCompare2(TIM3,181);
							
							num_count++;Food_count++;LCD_ClearLine(LINE(0));
							
							sprintf(dispBuff,"%d Food Scrap %d OK!",num_count,Food_count);
							
							ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;
							
			case 12: USART_SendData(DEBUG_USARTx,ucTemp);
							 delay_ms(500);TIM_SetCompare2(TIM3,187);delay_ms(500);TIM_SetCompare2(TIM3,181);
							 
							 num_count++;Other_count++;LCD_ClearLine(LINE(0));
							
						   sprintf(dispBuff,"%d Other waste %d OK!",num_count,Other_count);
							
							 ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;

			case 13: USART_SendData(DEBUG_USARTx,ucTemp);
							 delay_ms(500);TIM_SetCompare2(TIM3,187);delay_ms(500);TIM_SetCompare2(TIM3,181);
							 
							 num_count++;Other_count++;LCD_ClearLine(LINE(0));
							
						   sprintf(dispBuff,"%d Other waste %d OK!",num_count,Other_count);
							
							 ILI9341_DispStringLine_EN(LINE(0),dispBuff);break;

			default: USART_SendData(DEBUG_USARTx,ucTemp);delay_ms(5000);
			
							 LCD_ClearLine(LINE(0));LCD_ClearLine(LINE(1));
							 
							 ILI9341_DispStringLine_EN(LINE(0),"Please place garbage");
		}	
	}	 
}
