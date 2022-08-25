#include "Control.h"
#include "jidianqi.h" 
#include "delay.h"
#include "US100.h"
#include "usart.h"
#include "lcd.h"

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
//		LCD_ShowNum(260,260,(int)(X),5,12);
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
            sum2 = sum2 - Dis_Cam_Buf[k] + Dis_Cam_temp;		//进行滤波处理
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

void Shoot_Control(void)    //定电压
{
    jidianqi_state(2,1);    //放电
    delay_ms(1000);
    jidianqi_state(2,0);    //放电关
    jidianqi_state(1,1);    //充电
    delay_ms(3000);
    jidianqi_state(1,0);    //停止充电
    delay_ms(1000); 
    jidianqi_state(2,1);    //放电
    delay_ms(2000);
    jidianqi_state(2,0);    //停止放电
}

void Get_Dis(void)
{
    UltrasonicWave_StartMeasure();
}


