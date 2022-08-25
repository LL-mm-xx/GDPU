#include "PID.h"

/*******************************************************************************
//������PID���ƴ���(λ��ʽ)
//���ߣ��ܲ���
//CSDN���ܲ���ؼ
//�����������ܲ���ؼ
//�޸�����:2021��5��13��
//�汾��V1.0
********************************************************************************
//V1.0 2021/5/13
*******************************************************************************/

//ǰ��
float P=1,I=0.35,D=13,Set_Temp=60;				//��ʼ��P,I,D����ǰֵ������ֵ ����
float PID_OUT=0,PWM_Duty=0;   						//PID���           
float P_OUT=0,I_OUT=0,D_OUT=0;    				//������������������΢�����
float Current_Error=0, Last_Error=0;    	//��ǰ���  ������
float Sum_Error=0;     										//������ 
float PID_I_MAX=1000.0,PID_I_MIN=-1000.0,V_DATA_MAX=1000,V_DATA_MIN=0;

//ǰ��
float Set_Temp1=60;												//��ʼ��P,I,D����ǰֵ������ֵ ����
float PID_OUT1=0,PWM_Duty1=0;  					  //PID���           
float P_OUT1=0,I_OUT1=0,D_OUT1=0;   			//������������������΢�����
float Current_Error1=0, Last_Error1=0;    //��ǰ���  ������
float Sum_Error1=0;     									//������ 
float PID_I_MAX1=1000.0,PID_I_MIN1=-1000.0,V_DATA_MAX1=1000,V_DATA_MIN1=0;

//����
float Set_Temp2=60;												//��ʼ��P,I,D����ǰֵ������ֵ ����
float PID_OUT2=0,PWM_Duty2=0;    					//PID���           
float P_OUT2=0,I_OUT2=0,D_OUT2=0;   			//������������������΢�����
float Current_Error2=0, Last_Error2=0;    //��ǰ���  ������
float Sum_Error2=0;     									//������ 
float PID_I_MAX2=1000.0,PID_I_MIN2=-1000.0,V_DATA_MAX2=1000,V_DATA_MIN2=0;

//����
float Set_Temp3=60;												//��ʼ��P,I,D����ǰֵ������ֵ ����
float PID_OUT3=0,PWM_Duty3=0;   				  //PID���           
float P_OUT3=0,I_OUT3=0,D_OUT3=0;    			//������������������΢�����
float Current_Error3=0, Last_Error3=0;    //��ǰ���  ������
float Sum_Error3=0;    									  //������ 
float PID_I_MAX3=1000.0,PID_I_MIN3=-1000.0,V_DATA_MAX3=1000,V_DATA_MIN3=0;

void PID_calc(float V_1, float V_2, float V_3, float V_4) //PID�㷨
{
    float Rate;//���仯��
    float Rate1;//���仯��
		float Rate2;//���仯��
    float Rate3;//���仯��
   ///////////////////////////////ǰ����///////////////////////////////////
    Current_Error = Set_Temp - V_1;//��ǰ���
    Sum_Error += Current_Error; //������
    Last_Error = Current_Error;//�洢������
    Rate = Current_Error - Last_Error; //�仯���ʼ���

    if (Rate > 10) Rate = 10;//�����仯����-10��10֮��
    if (Rate < -10)	Rate = -10;

    P_OUT = P * Current_Error; //������
    I_OUT = I * Sum_Error; //������

    //�����޷�����
    if (I_OUT > PID_I_MAX)  I_OUT = PID_I_MAX; //���ܳ������ֵ���ܵ�����Сֵ
    if (I_OUT < PID_I_MIN)  I_OUT = PID_I_MIN;

    //΢���������
    D_OUT = D * Rate;
    PID_OUT =  P_OUT  +  I_OUT  +  D_OUT ;

    if (PID_OUT >= V_DATA_MAX)  PID_OUT = V_DATA_MAX;
    if (PID_OUT <= V_DATA_MIN)  PID_OUT = V_DATA_MIN;
		///////////////////////////////ǰ����///////////////////////////////////
    Current_Error1 = Set_Temp1 - V_2;//��ǰ���
    Sum_Error1 += Current_Error1; //������
    Last_Error1 = Current_Error1;//�洢������
    Rate1 = Current_Error1 - Last_Error1; //�仯���ʼ���

    if (Rate1 > 10) Rate1 = 10;//�����仯����-10��10֮��
    if (Rate1 < -10)	Rate1 = -10;

    P_OUT1 = P * Current_Error1; //������
    I_OUT1 = I * Sum_Error1; //������

    //�����޷�����
    if (I_OUT1 > PID_I_MAX1)  I_OUT1 = PID_I_MAX1; //���ܳ������ֵ���ܵ�����Сֵ
    if (I_OUT1 < PID_I_MIN1)  I_OUT1 = PID_I_MIN1;

    //΢���������
    D_OUT1 = D * Rate1;
    PID_OUT1 =  P_OUT1  +  I_OUT1  +  D_OUT1 ;
    if (PID_OUT1 >= V_DATA_MAX1)  PID_OUT1 = V_DATA_MAX1;
    if (PID_OUT1 <= V_DATA_MIN1)  PID_OUT1 = V_DATA_MIN1;	
    ///////////////////////////////������///////////////////////////////////
		Current_Error2 = Set_Temp2 - V_3;//��ǰ���
		Sum_Error2 +=Current_Error2;//������
		Last_Error2 = Current_Error2;//�洢������
		Rate2 = Current_Error2-Last_Error2;//�仯���ʼ���
			
    if (Rate2 > 10) Rate2 = 10;//�����仯����-10��10֮��
    if (Rate2 < -10)	Rate2 = -10;

		P_OUT2 = P * Current_Error2;//������
		I_OUT2 = I * Sum_Error2;//������

		//�����޷�����
		if( I_OUT2>PID_I_MAX2 )  I_OUT2 = PID_I_MAX2;//���ܳ������ֵ���ܵ�����Сֵ
		if( I_OUT2<PID_I_MIN2)  I_OUT2 = PID_I_MIN2;

		//΢���������
		D_OUT2 = D * Rate2;
		PID_OUT2 =  P_OUT2  +  I_OUT2  +  D_OUT2 ;
		if ( PID_OUT2 >= V_DATA_MAX2 )  PID_OUT2 = V_DATA_MAX2;
		if ( PID_OUT2 <= V_DATA_MIN2 )  PID_OUT2 = V_DATA_MIN2;	
		///////////////////////////////������///////////////////////////////////
		Current_Error3 = Set_Temp3 - V_4;//��ǰ���
		Sum_Error3 +=Current_Error3;//������
		Last_Error3 = Current_Error3;//�洢������
		Rate3 = Current_Error3-Last_Error3;//�仯���ʼ���
						
    if (Rate3 > 10) Rate3 = 10;//�����仯����-10��10֮��
    if (Rate3 < -10)	Rate3 = -10;
		
		P_OUT3 = P * Current_Error3;//������
		I_OUT3 = I * Sum_Error3;//������
		
		//�����޷�����
		if( I_OUT3>PID_I_MAX3 )  I_OUT3 = PID_I_MAX3;//���ܳ������ֵ���ܵ�����Сֵ
		if( I_OUT3<PID_I_MIN3 )  I_OUT3 = PID_I_MIN3;

		//΢���������
		D_OUT3 = D * Rate3;
		PID_OUT3 =  P_OUT3  +  I_OUT3  +  D_OUT3 ;
		if ( PID_OUT3 >= V_DATA_MAX3 )  PID_OUT3 = V_DATA_MAX3;
		if ( PID_OUT3 <= V_DATA_MIN3 )  PID_OUT3 = V_DATA_MIN3;
}



