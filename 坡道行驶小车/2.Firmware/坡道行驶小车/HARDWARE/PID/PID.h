#ifndef _PID_H
#define _PID_H


/*******************************************************************************
//������PID���ƴ���
//���ߣ�������
//CSDN��������
//����������������my
//�޸�����:2021��4��2��16:33:58
//�汾��V1.0								  
********************************************************************************
//V1.0 2021/4/2�	  
*******************************************************************************/
   	  
//ǰ���ֳ�ʼ������
extern float P,I,D,Set_Temp;								//��ʼ��P,I,D����ǰֵ������ֵ ����
extern float PID_OUT,PWM_Duty;    					//PID���           
extern float P_OUT,I_OUT,D_OUT;    					//������������������΢�����
extern float Current_Error, Last_Error;    	//��ǰ���  ������
extern float Sum_Error;     			//������ 
extern float PID_I_MAX,PID_I_MIN,V_DATA_MAX,V_DATA_MIN;

//ǰ���ֳ�ʼ������
extern float Set_Temp1;											//��ʼ��P,I,D����ǰֵ������ֵ ����
extern float PID_OUT1,PWM_Duty1;  				  //PID���           
extern float P_OUT1,I_OUT1,D_OUT1;  			  //������������������΢�����
extern float Current_Error1, Last_Error1;   //��ǰ���  ������
extern float Sum_Error1;     		//������ 
extern float PID_I_MAX1,PID_I_MIN1,V_DATA_MAX1,V_DATA_MIN1;

//�����ֳ�ʼ������
extern float Set_Temp2;											//��ʼ��P,I,D����ǰֵ������ֵ ����
extern float PID_OUT2,PWM_Duty2;    				//PID���           
extern float P_OUT2,I_OUT1,D_OUT2;   				//������������������΢�����
extern float Current_Error2, Last_Error2;   //��ǰ���  ������
extern float Sum_Error2;    	  //������ 
extern float PID_I_MAX2,PID_I_MIN2,V_DATA_MAX2,V_DATA_MIN2;

//�����ֳ�ʼ������
extern float Set_Temp3;											//��ʼ��P,I,D����ǰֵ������ֵ ����
extern float PID_OUT3,PWM_Duty3;   				  //PID���           
extern float P_OUT3,I_OUT3,D_OUT3;    			//������������������΢�����
extern float Current_Error3, Last_Error3;   //��ǰ���  ������
extern float Sum_Error3;    	  //������ 
extern float PID_I_MAX3,PID_I_MIN3,V_DATA_MAX3,V_DATA_MIN3;

void PID_calc(float V_1, float V_2, float V_3, float V_4); //PID�㷨

#endif


