#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "touch.h" 
#include "pwm.h"
#include "jidianqi.h"
#include "interface.h"
#include "timer.h"
#include "control.h"

uint8_t RxBuffer1[20];
__IO uint8_t RxCounter1 = 0x00; 
u8 RxFlag1 = 0;
u8 Ball_flag=0;

u16 X,Dis=0;      //标靶的信息
u16 Dis_Chao = 0;
u16 Dis_Cam = 0;

int Angle = 0;    //键盘输入水平转角
u8 Display_Flag = 1;
int MODE=10,Start=0;

int Receive_flag = 0;

int angle_V = 0,angle_H = 0;    //舵机转动的角度
int Switch_flag = 0;
u8 Dis_Num[3] = {0},Ang_Num[3] = {0};
u8 Angle_Neg_flag = 0;
int temp1 = 0;
u8 flag_flip = 0;
u8 find_flag = 0;
u8 Buf[100];

u8 Shoot_flag = 0;
u8 aaa = 1;		//每次发射后，aaa变为0，按return后，aaa重新置1，才能进行下一次发射
u16 t = 0;
u8 tt = 0;
u8 tt_chong_flag = 1;
u16 tt_chong = 0;
u16 tt_chong_flag_5 = 0;
u16 LastX,LastY=0;
int XSpeed,YSpeed=0;
float Err_X,Err_Y=0;
float Err_X_LAST,Err_Y_LAST=0;
float Spd_Err_X,Spd_Err_Y=0;
float Spd_Err_X_LAST,Spd_Err_Y_LAST=0;

int X_Static_PWM=X_MID,Y_Static_PWM=Y_MID;
int PWM_X = X_MID,PWM_Y = Y_MID;          //pid参数初始化

int POSX_Integral,POSY_Integral=0;
int Spd_Aim_X,Spd_Aim_Y=0;
int Aim_X=185,Aim_Y=120;

float   Kp=-0.9,           Ki=-0.1,           Kd=0;         //Pan

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  		//初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	TIM3_Int_Init(200-1,8400-1);//10Khz计数  50Hz中断	
	TIM4_PWM_Init(20000-1,84-1);        //50Hz 20ms
	TIM_SetCompare2(TIM4,1300);	
	TIM_SetCompare1(TIM4,1680);	
	jidianqi_Init();
	LCD_Init();					//LCD初始化  
	tp_dev.init();				//触摸屏初始化
 	POINT_COLOR=RED;//设置字体为红色 

	while(1)
	{
		
		 switch(MODE)
     {
				case MODE1:
        Mode1_Select_Display();//数字输入界面
        break;
        case MODE2:
        Mode2_Select_Display();//自动跟随
        break;				
        default:
        Mode_Select_Display();//主界面
        break;        
     } 
     if(Receive_flag) 
     {
					USART2_Rx_Task();
     }		 
		 Debug_Data_Display(); 
		 
     if(Shoot_flag == 1&&aaa == 1)
     {
         Shoot_Control();
         aaa = 0;		 
		 }
	 }
}
