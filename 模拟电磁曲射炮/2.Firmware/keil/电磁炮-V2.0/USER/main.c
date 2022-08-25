#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h" 
#include "usart2.h"  
#include "timer.h"  
#include "string.h"
#include "pwm.h"
#include "Control.h"
#include "interface.h"
#include "US100.h"
#include "touch.h"
#include "jidianqi.h"

int Calculate(u16 distance);
void Turn_Angle(float Angle,u8 Servo);

uint8_t RxBuffer1[20];
__IO uint8_t RxCounter1 = 0x00; 
u8 RxFlag1 = 0;
u8 Ball_flag=0;
u16 X,Dis=0;      //��е���Ϣ
u16 Dis_Chao = 0;
u16 Dis_Cam = 0;
int Angle = 0;      //��������ˮƽת��
u8 Display_Flag = 1;
int MODE=10,Start=0;
int Receive_flag = 0;
int angle_V = 0,angle_H = 0;    //���ת���ĽǶ�
int Switch_flag = 0;
u8 Dis_Num[3] = {0},Ang_Num[3] = {0};
u8 Angle_Neg_flag = 0;
int temp1 = 0;
u8 flag_flip = 0;
u8 find_flag = 0;
u8 Buf[100];

u8 Shoot_flag = 0;
u8 aaa = 1;
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
int PWM_X = X_MID,PWM_Y = Y_MID;          //pid������ʼ��

int POSX_Integral,POSY_Integral=0;
int Spd_Aim_X,Spd_Aim_Y=0;
int Aim_X=183,Aim_Y=108;

float   Kp=-0.15,           Ki=-0.08,           Kd=0;         //Pan

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200 
	usart2_init(42,115200);		//��ʼ������2������Ϊ115200
	TIM3_Int_Init(200-1,8400-1);//10Khz����  50Hz�ж�	
	TIM4_PWM_Init(20000-1,84-1);        //50Hz 20ms
	TIM_SetCompare2(TIM4,1300);	
	TIM_SetCompare1(TIM4,1680);	
//	UltrasonicWave_Timerx_Init();
//  UltrasonicWave_Configuration();
	jidianqi_Init();
 	LCD_Init();					//LCD��ʼ��  
  tp_dev.init();  //��������ʼ��&24C02
 	POINT_COLOR=RED;//��������Ϊ��ɫ  	 

    while(1)
    {
        switch(MODE)
        {
            case MODE1:
                Mode1_Select_Display();//�����������
                break;
            case MODE2:
                Mode2_Select_Display();//�Զ�����
                break;
            case MODE3:
                Mode3_Select_Display();//����ת������30��
                break;
            default:
                Mode_Select_Display();//������
                break;        
        }     
        if(Receive_flag) 
        {
            USART2_Rx_Task();
            UltrasonicWave_StartMeasure();
        }
        Debug_Data_Display(); 
        
        if(Shoot_flag == 1&&aaa == 1)
        {
            Shoot_Control();
            aaa = 0;
        }
    }
}

void TIM3_IRQHandler(void)           //�����жϺ��� 50Hz
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
        switch(MODE)
        {
            case MODE1:
                if(Start){
                    angle_V = Calculate(Dis);
                    angle_H = Angle;
                    Turn_Angle(angle_V,1);
                    Turn_Angle(angle_H,2);
                    Shoot_flag = 1;
                }
                else {
                angle_V = 0;angle_H = 0;
                TIM_SetCompare1(TIM4,X_MID);
                TIM_SetCompare2(TIM4,Y_MID);
                  //Turn_Angle(0,1);
                }
                break;
                
            case MODE2:
                if(Start == 1)      //����Cam�ľ������
                {
                    if(Ball_flag == 0)  //���û���ҵ�����
                    {
                         tt++;
                         if(flag_flip){
                            if(tt == 50) {angle_H += 5;tt = 0;} 
                            if(angle_H >= 20) flag_flip = 0;
                        }else{
                            if(tt == 50) {angle_H -= 5;tt = 0;} 
                            if(angle_H <= -20) flag_flip = 1;
                        }
                        Turn_Angle(angle_H,2);
                    }
                    else 
                    {
                        IncPID_Pos();                   //��������ƶ�����
                        if(PWM_X < 1800 && PWM_X > 1500) TIM_SetCompare1(TIM4,PWM_X);	//�������PID����ֵ
                        angle_H = (PWM_X - X_MID)*0.135;   //��ʾ��
                        angle_V = Calculate(Dis_Cam-5);           
                        if(angle_V > 0)Turn_Angle(angle_V,1);                        
                        if(fabs(X-Aim_X)<4) t++;
                        if(t == 250) Shoot_flag = 1;
                    }
                }
                
                else if(Start == 2)         //����Chao�ľ������
                {
                    if(Ball_flag == 0)      //���û���ҵ�����
                    {
                         tt++;
                         if(flag_flip){
                            if(tt == 50) {angle_H += 5;tt = 0;} 
                            if(angle_H >= 20) flag_flip = 0;
                        }else{
                            if(tt == 50) {angle_H -= 5;tt = 0;} 
                            if(angle_H <= -20) flag_flip = 1;
                        }
                        Turn_Angle(angle_H,2);
                    }
                    else 
                    {
                        IncPID_Pos();                   //��������ƶ�����
                        //TIM_SetCompare1(TIM4,PWM_X);	//�������PID����ֵ
                        if(PWM_X < 1902 && PWM_X > 661) TIM_SetCompare1(TIM4,PWM_X);	//�������PID����ֵ
                        angle_H = (PWM_X - X_MID)*0.135;   //��ʾ��
                        angle_V = Calculate(Dis_Cam);
												LCD_ShowNum(280,360,(int)(Dis_Cam),3,12);
                        if(angle_V > 0)Turn_Angle(angle_V,1);
                        if(fabs(X-Aim_X)<4) t++;
                        if(t == 250) Shoot_flag = 1;
                    }
                    
                }
                else {
                    angle_V = 0;angle_H = 0;
                    TIM_SetCompare1(TIM4,X_MID);
                    TIM_SetCompare2(TIM4,Y_MID);
                }
                break;
                
            case MODE3:
                if(Start){
                    Turn_Angle(20,1);       //�����ǵ��ù̶�
                    if(tt_chong_flag == 1)
                    {
                        jidianqi_state(2,1);    //�ŵ� 
                        tt_chong++; if(tt_chong == 20){tt_chong = 0;tt_chong_flag = 2;} //��ʱ
                    }   
                    else if(tt_chong_flag == 2)
                    {
                        jidianqi_state(2,0);    //�ŵ��
                        jidianqi_state(1,1);    //���
                        tt_chong++; if(tt_chong == 150){tt_chong = 0;tt_chong_flag = 3;}
                    }
                    else if(tt_chong_flag == 3)
                    {
                        jidianqi_state(1,0);    //ֹͣ���
                        tt_chong++; if(tt_chong == 100){tt_chong = 0;tt_chong_flag = 4;}
                    }   
                    else if(tt_chong_flag == 5) {jidianqi_state(2,0);tt_chong_flag_5 = 0;}    //ֹͣ�ŵ�
                    
                    if(find_flag == 0)
                    {   
                        tt++;
                        if(flag_flip){
                            if(tt == 6) {angle_H += 2;tt = 0;} 
                            if(angle_H >= 30) flag_flip = 0;
                        }else{
                            if(tt == 6) {angle_H -= 2;tt = 0;} 
                            if(angle_H <= -30) flag_flip = 1;
                        }
                        
                        if(fabs(X-Aim_X)<55) 
                        {
                            if(tt_chong_flag == 4) 
                            {
                                jidianqi_state(2,1);    //�ŵ�
                                tt_chong_flag_5 = 1;
                            }
                        }
                        
                        if(tt_chong_flag_5)
                        {
                            tt_chong++; if(tt_chong == 100){tt_chong = 0;tt_chong_flag = 5;}
                        }
                        
                        Turn_Angle(angle_H,2);
                    }
                }
                else
                {
                    angle_V = 0;angle_H = -30;
                    Turn_Angle(angle_V,1);
                    Turn_Angle(angle_H,2);
                }
                break;
            default:
                angle_V = 0;angle_H = 0;
                TIM_SetCompare1(TIM4,X_MID);
                TIM_SetCompare2(TIM4,Y_MID);
                break;
        }
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}

void Turn_Angle(float Angle,u8 Servo)
{   
    //2000��Ӧ270�� 0.135��/1
    if(Angle < 40 && Angle > -40)
    {
        if(Servo == 1) TIM_SetCompare2(TIM4,(int)(Y_MID + Angle/0.135f));      //0.153
        else if(Servo == 2) TIM_SetCompare1(TIM4,(int)(X_MID + Angle/0.135f));
    }
}

int Calculate(u16 distance) //��������������
{
    float Dist = distance/100.0;
		float theta = 0;
		if(Dist != 0)	theta = 0.1805f*pow(Dist,5) -2.117f*pow(Dist,4) + 10.41f*pow(Dist,3) -26.56f*pow(Dist,2) + 44.62f*Dist - 28.05f;
    return theta;   //�Ƕ�ֵ
}





