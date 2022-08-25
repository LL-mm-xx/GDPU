#include "PID.h"

/*******************************************************************************
//编码电机PID控制代码(位置式)
//作者：萝卜明
//CSDN：萝卜明丶
//哔哩哔哩：萝卜明丶
//修改日期:2021年5月13日
//版本：V1.0
********************************************************************************
//V1.0 2021/5/13
*******************************************************************************/

//前左
float P=1,I=0.35,D=13,Set_Temp=60;				//初始化P,I,D，当前值，设置值 常数
float PID_OUT=0,PWM_Duty=0;   						//PID输出           
float P_OUT=0,I_OUT=0,D_OUT=0;    				//比例输出，积分输出，微分输出
float Current_Error=0, Last_Error=0;    	//当前误差  最后误差
float Sum_Error=0;     										//误差积分 
float PID_I_MAX=1000.0,PID_I_MIN=-1000.0,V_DATA_MAX=1000,V_DATA_MIN=0;

//前右
float Set_Temp1=60;												//初始化P,I,D，当前值，设置值 常数
float PID_OUT1=0,PWM_Duty1=0;  					  //PID输出           
float P_OUT1=0,I_OUT1=0,D_OUT1=0;   			//比例输出，积分输出，微分输出
float Current_Error1=0, Last_Error1=0;    //当前误差  最后误差
float Sum_Error1=0;     									//误差积分 
float PID_I_MAX1=1000.0,PID_I_MIN1=-1000.0,V_DATA_MAX1=1000,V_DATA_MIN1=0;

//后左
float Set_Temp2=60;												//初始化P,I,D，当前值，设置值 常数
float PID_OUT2=0,PWM_Duty2=0;    					//PID输出           
float P_OUT2=0,I_OUT2=0,D_OUT2=0;   			//比例输出，积分输出，微分输出
float Current_Error2=0, Last_Error2=0;    //当前误差  最后误差
float Sum_Error2=0;     									//误差积分 
float PID_I_MAX2=1000.0,PID_I_MIN2=-1000.0,V_DATA_MAX2=1000,V_DATA_MIN2=0;

//后右
float Set_Temp3=60;												//初始化P,I,D，当前值，设置值 常数
float PID_OUT3=0,PWM_Duty3=0;   				  //PID输出           
float P_OUT3=0,I_OUT3=0,D_OUT3=0;    			//比例输出，积分输出，微分输出
float Current_Error3=0, Last_Error3=0;    //当前误差  最后误差
float Sum_Error3=0;    									  //误差积分 
float PID_I_MAX3=1000.0,PID_I_MIN3=-1000.0,V_DATA_MAX3=1000,V_DATA_MIN3=0;

void PID_calc(float V_1, float V_2, float V_3, float V_4) //PID算法
{
    float Rate;//误差变化率
    float Rate1;//误差变化率
		float Rate2;//误差变化率
    float Rate3;//误差变化率
   ///////////////////////////////前左轮///////////////////////////////////
    Current_Error = Set_Temp - V_1;//当前误差
    Sum_Error += Current_Error; //误差积分
    Last_Error = Current_Error;//存储误差分析
    Rate = Current_Error - Last_Error; //变化速率计算

    if (Rate > 10) Rate = 10;//令误差变化率在-10到10之间
    if (Rate < -10)	Rate = -10;

    P_OUT = P * Current_Error; //比列项
    I_OUT = I * Sum_Error; //积分项

    //积分限幅处理
    if (I_OUT > PID_I_MAX)  I_OUT = PID_I_MAX; //不能超过最大值不能低于最小值
    if (I_OUT < PID_I_MIN)  I_OUT = PID_I_MIN;

    //微分输出处理
    D_OUT = D * Rate;
    PID_OUT =  P_OUT  +  I_OUT  +  D_OUT ;

    if (PID_OUT >= V_DATA_MAX)  PID_OUT = V_DATA_MAX;
    if (PID_OUT <= V_DATA_MIN)  PID_OUT = V_DATA_MIN;
		///////////////////////////////前右轮///////////////////////////////////
    Current_Error1 = Set_Temp1 - V_2;//当前误差
    Sum_Error1 += Current_Error1; //误差积分
    Last_Error1 = Current_Error1;//存储误差分析
    Rate1 = Current_Error1 - Last_Error1; //变化速率计算

    if (Rate1 > 10) Rate1 = 10;//令误差变化率在-10到10之间
    if (Rate1 < -10)	Rate1 = -10;

    P_OUT1 = P * Current_Error1; //比列项
    I_OUT1 = I * Sum_Error1; //积分项

    //积分限幅处理
    if (I_OUT1 > PID_I_MAX1)  I_OUT1 = PID_I_MAX1; //不能超过最大值不能低于最小值
    if (I_OUT1 < PID_I_MIN1)  I_OUT1 = PID_I_MIN1;

    //微分输出处理
    D_OUT1 = D * Rate1;
    PID_OUT1 =  P_OUT1  +  I_OUT1  +  D_OUT1 ;
    if (PID_OUT1 >= V_DATA_MAX1)  PID_OUT1 = V_DATA_MAX1;
    if (PID_OUT1 <= V_DATA_MIN1)  PID_OUT1 = V_DATA_MIN1;	
    ///////////////////////////////后左轮///////////////////////////////////
		Current_Error2 = Set_Temp2 - V_3;//当前误差
		Sum_Error2 +=Current_Error2;//误差积分
		Last_Error2 = Current_Error2;//存储误差分析
		Rate2 = Current_Error2-Last_Error2;//变化速率计算
			
    if (Rate2 > 10) Rate2 = 10;//令误差变化率在-10到10之间
    if (Rate2 < -10)	Rate2 = -10;

		P_OUT2 = P * Current_Error2;//比列项
		I_OUT2 = I * Sum_Error2;//积分项

		//积分限幅处理
		if( I_OUT2>PID_I_MAX2 )  I_OUT2 = PID_I_MAX2;//不能超过最大值不能低于最小值
		if( I_OUT2<PID_I_MIN2)  I_OUT2 = PID_I_MIN2;

		//微分输出处理
		D_OUT2 = D * Rate2;
		PID_OUT2 =  P_OUT2  +  I_OUT2  +  D_OUT2 ;
		if ( PID_OUT2 >= V_DATA_MAX2 )  PID_OUT2 = V_DATA_MAX2;
		if ( PID_OUT2 <= V_DATA_MIN2 )  PID_OUT2 = V_DATA_MIN2;	
		///////////////////////////////后右轮///////////////////////////////////
		Current_Error3 = Set_Temp3 - V_4;//当前误差
		Sum_Error3 +=Current_Error3;//误差积分
		Last_Error3 = Current_Error3;//存储误差分析
		Rate3 = Current_Error3-Last_Error3;//变化速率计算
						
    if (Rate3 > 10) Rate3 = 10;//令误差变化率在-10到10之间
    if (Rate3 < -10)	Rate3 = -10;
		
		P_OUT3 = P * Current_Error3;//比列项
		I_OUT3 = I * Sum_Error3;//积分项
		
		//积分限幅处理
		if( I_OUT3>PID_I_MAX3 )  I_OUT3 = PID_I_MAX3;//不能超过最大值不能低于最小值
		if( I_OUT3<PID_I_MIN3 )  I_OUT3 = PID_I_MIN3;

		//微分输出处理
		D_OUT3 = D * Rate3;
		PID_OUT3 =  P_OUT3  +  I_OUT3  +  D_OUT3 ;
		if ( PID_OUT3 >= V_DATA_MAX3 )  PID_OUT3 = V_DATA_MAX3;
		if ( PID_OUT3 <= V_DATA_MIN3 )  PID_OUT3 = V_DATA_MIN3;
}



