#ifndef _PID_H
#define _PID_H


/*******************************************************************************
//编码电机PID控制代码
//作者：化作尘
//CSDN：化作尘
//哔哩哔哩：化作尘my
//修改日期:2021年4月2日16:33:58
//版本：V1.0								  
********************************************************************************
//V1.0 2021/4/2?	  
*******************************************************************************/
   	  
//前左轮初始化参数
extern float P,I,D,Set_Temp;								//初始化P,I,D，当前值，设置值 常数
extern float PID_OUT,PWM_Duty;    					//PID输出           
extern float P_OUT,I_OUT,D_OUT;    					//比例输出，积分输出，微分输出
extern float Current_Error, Last_Error;    	//当前误差  最后误差
extern float Sum_Error;     			//误差积分 
extern float PID_I_MAX,PID_I_MIN,V_DATA_MAX,V_DATA_MIN;

//前右轮初始化参数
extern float Set_Temp1;											//初始化P,I,D，当前值，设置值 常数
extern float PID_OUT1,PWM_Duty1;  				  //PID输出           
extern float P_OUT1,I_OUT1,D_OUT1;  			  //比例输出，积分输出，微分输出
extern float Current_Error1, Last_Error1;   //当前误差  最后误差
extern float Sum_Error1;     		//误差积分 
extern float PID_I_MAX1,PID_I_MIN1,V_DATA_MAX1,V_DATA_MIN1;

//后左轮初始化参数
extern float Set_Temp2;											//初始化P,I,D，当前值，设置值 常数
extern float PID_OUT2,PWM_Duty2;    				//PID输出           
extern float P_OUT2,I_OUT1,D_OUT2;   				//比例输出，积分输出，微分输出
extern float Current_Error2, Last_Error2;   //当前误差  最后误差
extern float Sum_Error2;    	  //误差积分 
extern float PID_I_MAX2,PID_I_MIN2,V_DATA_MAX2,V_DATA_MIN2;

//后右轮初始化参数
extern float Set_Temp3;											//初始化P,I,D，当前值，设置值 常数
extern float PID_OUT3,PWM_Duty3;   				  //PID输出           
extern float P_OUT3,I_OUT3,D_OUT3;    			//比例输出，积分输出，微分输出
extern float Current_Error3, Last_Error3;   //当前误差  最后误差
extern float Sum_Error3;    	  //误差积分 
extern float PID_I_MAX3,PID_I_MIN3,V_DATA_MAX3,V_DATA_MIN3;

void PID_calc(float V_1, float V_2, float V_3, float V_4); //PID算法

#endif


