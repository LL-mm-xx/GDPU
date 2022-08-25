#include "control.h"
#include "jidianqi.h" 
#include "delay.h"
#include "math.h"

//发射控制
void Shoot_Control(void)    //定电压
{
	jidianqi_state(2,1);    //放电开
	delay_ms(1000);
	jidianqi_state(2,0);    //放电关
	jidianqi_state(1,1);    //充电开
	delay_ms(4000);
	jidianqi_state(1,0);    //充电关
	delay_ms(2000);
	jidianqi_state(2,1);    //放电开
	delay_ms(2000);
	jidianqi_state(2,0);    //放电关
}

 //控制转向
void Turn_Angle(float Angle,u8 Servo)
{   
		//1为x，2为y
		//中间角度：通道1为1680，通道2为1300
		//2000对应270° 0.135°/1   2000对应180°  0.09°/1
    if(Angle < 40 && Angle > -40)
    {
        if(Servo == 1) TIM_SetCompare2(TIM4,(int)(Y_MID + Angle/0.09f));      
        else if(Servo == 2) TIM_SetCompare1(TIM4,(int)(X_MID + Angle/0.09f));
    }
}

int Calculate(u16 distance) //输入距离计算仰角
{
    float Dist = distance/100.0f;
		float theta = 0;
		if(Dist != 0)	theta = 0.1315f*pow(Dist,5) - 1.821f*pow(Dist,4) + 10.28f*pow(Dist,3) -29.6f*pow(Dist,2) + 50.85f*Dist - 34.38f - 5;
    return theta;   //角度值
}

//串口2接收OpenMV数据函数
void USART2_Rx_Task(void)
{
    static u16 Dis_Cam_temp = 0;
    static u16 sum2 = 0;
    static u16 Dis_Cam_Buf[10] = {0};
    static u8 k = 0;
	if(RxFlag1 == 1)
	{
		X = RxBuffer1[3]<<8 | RxBuffer1[2];
        if((X == 10000)) 
        {
            Ball_flag=0;
            RxFlag1 = 0;RxCounter1 = 0;		
            USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);      	
            return;
        }
        else 
        {
            Ball_flag=1;
        //    Dis_Cam = RxBuffer1[5]<<8 | RxBuffer1[4];
            Dis_Cam_temp = RxBuffer1[5]<<8 | RxBuffer1[4];
            sum2 = sum2 - Dis_Cam_Buf[k] + Dis_Cam_temp;
            Dis_Cam_Buf[k] = Dis_Cam_temp;
            k++;if(k>9) k = 0;
            Dis_Cam = sum2/10;
        }
              
		RxFlag1 = 0;RxCounter1 = 0;		
	}
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);      	
}

//位置环增量式PID控制
void IncPID_Pos(void)
{
    static int Beta_X = 1;
    static float dError_X = 0,Err_X_PRE = 0;  
    static float PosInc_PWM_X,Position_PWM_X = X_MID;
    if(Ball_flag)
    {
        Err_X=X-Aim_X;	    //计算误差 
        if(fabs(Err_X)<3) Err_X = 0;      //带死区的PID算法
        //Beta_X = (fabs(Err_X)>120)?0:1;       //积分分离 偏差过大积分效果消除
        
        //变积分式PID
//        if(fabs(Err_X)<10) Beta_X = 1;
//        else if(fabs(Err_X)>110) Beta_X = 0;
//        else Beta_X = (110 - fabs(Err_X))/(110 - 10);
        
        //积分抗饱和
        POSX_Integral = (Err_X + Err_X_LAST)/2;
        if(POSX_Integral>80)POSX_Integral=80;
        if(POSX_Integral<-80)POSX_Integral=-80;
        
        dError_X=Kd*(Err_X - 2*Err_X_LAST + Err_X_PRE)*0.4f + dError_X*0.6f;  //不完全微分式低通滤波 去除高频干扰
        
        PosInc_PWM_X = Kp*(Err_X - Err_X_LAST) + Beta_X*Ki*(POSX_Integral) + (dError_X);
        Err_X_PRE = Err_X_LAST; Err_X_LAST = Err_X;
        Position_PWM_X += PosInc_PWM_X;
        PWM_X = Position_PWM_X;
    }     
} 

