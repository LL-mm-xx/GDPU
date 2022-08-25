#include "interface.h"
#include "lcd.h"
#include "touch.h"
#include "math.h"

void Mode_Select_Display(void)
{
    tp_dev.scan(0);
    if(70<tp_dev.x[0] && tp_dev.x[0]<134 && 50<tp_dev.y[0] && tp_dev.y[0]<75 && Start==0)//MODE1
    {
        MODE = MODE1;
        Display_Flag = 2;
        while(70<tp_dev.x[0] && tp_dev.x[0]<134 && 50<tp_dev.y[0] && tp_dev.y[0]<75)tp_dev.scan(0);
    }
    if(70<tp_dev.x[0] && tp_dev.x[0]<134 && 90<tp_dev.y[0] && tp_dev.y[0]<115 && Start==0)//MODE2
    {
        MODE = MODE2;
        Display_Flag = 3;
        while(70<tp_dev.x[0] && tp_dev.x[0]<134 && 90<tp_dev.y[0] && tp_dev.y[0]<115)tp_dev.scan(0);
    }
    if(70<tp_dev.x[0] && tp_dev.x[0]<134 && 130<tp_dev.y[0] && tp_dev.y[0]<155 && Start==0)//MODE3
    {
        MODE = MODE3;
        Display_Flag = 4;
        while(70<tp_dev.x[0] && tp_dev.x[0]<134 && 130<tp_dev.y[0] && tp_dev.y[0]<155)tp_dev.scan(0);
    }
    
    if(Display_Flag==1)
    {
        Display_Flag=0;
        LCD_Fill(0,0,240,320,BLACK);//ÏÔÊ¾ÇøÓòÌî³äºÚÉ«
        POINT_COLOR = WHITE;
        BACK_COLOR=RED;  //±³¾°É« 
        LCD_ShowString(70,50,70,12,24," MODE1 ");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(15, 745, 95, 779);

        BACK_COLOR=BLUE;  //±³¾°É« 
        LCD_ShowString(70,90,70,12,24," MODE2 ");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(205, 745, 275, 779);

        BACK_COLOR=GRAY;
        LCD_ShowString(70,130,70,12,24," MODE3 ");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(395, 745, 465, 779);

        BACK_COLOR=BLACK;  //±³¾°É« 
        POINT_COLOR=WHITE;	//»­±ÊÑÕÉ«
    }
}

void Mode1_Select_Display(void)
{
     //´¥ÆÁ°´Å¥
    tp_dev.scan(0);
    if(10<tp_dev.x[0] && tp_dev.x[0]<74 && 10<tp_dev.y[0] && tp_dev.y[0]<35 && Start==0)//MODE1,HunUP
    {
        switch(Switch_flag)
        {
            case 0:
                Dis_Num[2]++;if(Dis_Num[2]>=5) Dis_Num[2] = 5;break;
            case 1:
                Ang_Num[2]++;if(Ang_Num[2]>=0) Ang_Num[2] = 0;break;
        }
        
        while(10<tp_dev.x[0] && tp_dev.x[0]<74 && 10<tp_dev.y[0] && tp_dev.y[0]<35)tp_dev.scan(0);
    }
    if(90<tp_dev.x[0] && tp_dev.x[0]<154 && 10<tp_dev.y[0] && tp_dev.y[0]<35 && Start==0)//MODE2,TenUP
    {
        switch(Switch_flag)
        {
            case 0:
                Dis_Num[1]++;if(Dis_Num[1]>=9) Dis_Num[1] = 9;break;
            case 1:
                Ang_Num[1]++;if(Ang_Num[1]>=3) Ang_Num[1] = 3;break;
        }
        
        while(90<tp_dev.x[0] && tp_dev.x[0]<154 && 10<tp_dev.y[0] && tp_dev.y[0]<35)tp_dev.scan(0);
    }
    if(170<tp_dev.x[0] && tp_dev.x[0]<234 && 10<tp_dev.y[0] && tp_dev.y[0]<35 && Start==0)//MODE3,NumUP
    {
        switch(Switch_flag)
        {
            case 0:
                Dis_Num[0]++;if(Dis_Num[0]>=9) Dis_Num[0] = 9;break;
            case 1:
                Ang_Num[0]++;if(Ang_Num[0]>=9) Ang_Num[0] = 9;break;
        }
        
        while(170<tp_dev.x[0] && tp_dev.x[0]<234 && 10<tp_dev.y[0] && tp_dev.y[0]<35)tp_dev.scan(0);
    }
    
    if(10<tp_dev.x[0] && tp_dev.x[0]<74 && 40<tp_dev.y[0] && tp_dev.y[0]<65 && Start==0)//MODE4,HunDN
    {
        switch(Switch_flag)
        {
            case 0:
                Dis_Num[2]--;if(Dis_Num[2]<=0) Dis_Num[2] = 0;break;
            case 1:
                Ang_Num[2]--;if(Ang_Num[2]<=0) Ang_Num[2] = 0;break;
        }
        
        while(10<tp_dev.x[0] && tp_dev.x[0]<74 && 40<tp_dev.y[0] && tp_dev.y[0]<65)tp_dev.scan(0);
    }
    
    if(90<tp_dev.x[0] && tp_dev.x[0]<154 && 40<tp_dev.y[0] && tp_dev.y[0]<65 && Start==0)//MODE5,TenDN
    {
        switch(Switch_flag)
        {
            case 0:
                Dis_Num[1]--;if(Dis_Num[1]<=0) Dis_Num[1] = 0;break;
            case 1:
                Ang_Num[1]--;if(Ang_Num[1]<=0) Ang_Num[1] = 0;break;
        }
        
        while(90<tp_dev.x[0] && tp_dev.x[0]<154 && 40<tp_dev.y[0] && tp_dev.y[0]<65)tp_dev.scan(0);
    }
    if(170<tp_dev.x[0] && tp_dev.x[0]<234 && 40<tp_dev.y[0] && tp_dev.y[0]<65 && Start==0)//MODE6,NumDN
    {
        switch(Switch_flag)
        {
            case 0:
                Dis_Num[0]--;if(Dis_Num[0]<=0) Dis_Num[0] = 0;break;
            case 1:
                Ang_Num[0]--;if(Ang_Num[0]<=0) Ang_Num[0] = 0;break;
        }     
        while(170<tp_dev.x[0] && tp_dev.x[0]<234 && 40<tp_dev.y[0] && tp_dev.y[0]<65)tp_dev.scan(0);
    }
    
    if(10<tp_dev.x[0] && tp_dev.x[0]<74 && 70<tp_dev.y[0] && tp_dev.y[0]<95 && Start==0)//Switch
    {
        Switch_flag = (Switch_flag==1)?0:1;
        while(10<tp_dev.x[0] && tp_dev.x[0]<74 && 70<tp_dev.y[0] && tp_dev.y[0]<95)tp_dev.scan(0);
    }
    if(170<tp_dev.x[0] && tp_dev.x[0]<234 && 70<tp_dev.y[0] && tp_dev.y[0]<95 && Start==0)//Start
    {
        Start = 1;
        while(90<tp_dev.x[0] && tp_dev.x[0]<154 && 70<tp_dev.y[0] && tp_dev.y[0]<95)tp_dev.scan(0);
    }
    
    if(10<tp_dev.x[0] && tp_dev.x[0]<74 && 100<tp_dev.y[0] && tp_dev.y[0]<125 )//Return 
    {
        Dis_Num[2] = Dis_Num[1] = Dis_Num[0] = 0;Dis = 0;
        Ang_Num[2] = Ang_Num[1] = Ang_Num[0] = 0;Angle = 0;
        Start = 0;
        MODE = 10;
        Display_Flag = 1;
        Shoot_flag = 0;
        aaa = 1;
        while(10<tp_dev.x[0] && tp_dev.x[0]<74 && 100<tp_dev.y[0] && tp_dev.y[0]<125)tp_dev.scan(0);
    }
    if(170<tp_dev.x[0] && tp_dev.x[0]<234 && 100<tp_dev.y[0] && tp_dev.y[0]<125 && Start==0)//Negti,ÇÐ»»¸º½Ç¶È
    {
         switch(Switch_flag)
        {
            case 0:
                break;
            case 1:
                Angle_Neg_flag = !Angle_Neg_flag;break;
        }
        while(170<tp_dev.x[0] && tp_dev.x[0]<234 && 100<tp_dev.y[0] && tp_dev.y[0]<125)tp_dev.scan(0);
    }
    
    Dis = Dis_Num[2]*100 + Dis_Num[1]*10 + Dis_Num[0]*1;		//¾àÀë
    temp1 = Ang_Num[2]*100 + Ang_Num[1]*10 + Ang_Num[0]*1;	//½Ç¶È
    if(Angle_Neg_flag) {temp1 = - temp1;}
    Angle = temp1;
    
    //ÏÔÊ¾½çÃæË¢ÐÂÒ»ÏÂ
    if(Display_Flag==2)
    {
        Display_Flag=0;
        LCD_Fill(0,0,240,320,BLACK);//ÏÔÊ¾ÇøÓòÌî³äºÚÉ«
        //DEBUG_MODE  °´Å¥
        POINT_COLOR=WHITE;	//»­±ÊÑÕÉ«
        BACK_COLOR=BROWN;  //À¶É«±³¾°É« 
        LCD_ShowString(10,10 ,50,12,24,"HunUP");//´¥ÃþÑ¡Ôñ   ³¤64 ¿í25
        //LCD_DrawRectangle(5, 5, 115, 229);
        
        LCD_ShowString(90,10 ,50,12,24,"TenUP");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(145, 195, 215, 229);
          
        LCD_ShowString(170,10 ,50,12,24,"NumUP");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(245, 195, 315, 229);

        BACK_COLOR=BLUE;  //ºìÉ«±³¾°É« 
        LCD_ShowString(10,40 ,50,12,24,"HunDN");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(345, 195, 415, 229);
        
        LCD_ShowString(90,40 ,50,12,24,"TenDN");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(45, 395, 115, 429);
        
        LCD_ShowString(170,40 ,50,12,24,"NumDN");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(245, 395, 315, 429);
        
        BACK_COLOR=RED;  //ºìÉ«±³¾°É« 
        LCD_ShowString(10,70 ,50,12,24,"Swtch");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(145, 395, 215, 429);
        
        LCD_ShowString(90,70 ,50,12,24,"");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(345, 395, 415, 429);
        
        BACK_COLOR=BLUE;  //ºìÉ«±³¾°É« 
        LCD_ShowString(170,70 ,50,12,24,"Start");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(15, 745, 95, 779);
        
        BACK_COLOR=RED;  //ºìÉ«±³¾°É« 
        LCD_ShowString(10,100 ,50,12,24,"Retrn");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(145, 395, 215, 429);
        
        LCD_ShowString(90,100 ,50,12,24,"");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(345, 395, 415, 429);
        
        BACK_COLOR=BLUE;  //ºìÉ«±³¾°É« 
        LCD_ShowString(170,100 ,50,12,24,"Negti");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(15, 745, 95, 779);
      
        BACK_COLOR=BLACK;  //±³¾°É« 
        POINT_COLOR=WHITE;	//»­±ÊÑÕÉ«
    }  
}

void Mode2_Select_Display(void)
{
     //´¥ÆÁ°´Å¥
    tp_dev.scan(0);
    if(90<tp_dev.x[0] && tp_dev.x[0]<154 && 70<tp_dev.y[0] && tp_dev.y[0]<95 && Start==0)//Start 2
    {
        Start = 1;
        while(90<tp_dev.x[0] && tp_dev.x[0]<154 && 70<tp_dev.y[0] && tp_dev.y[0]<95) tp_dev.scan(0);
    }
    
    if(170<tp_dev.x[0] && tp_dev.x[0]<234 && 70<tp_dev.y[0] && tp_dev.y[0]<95 && Start==0)//Start 3
    {
        Start = 2;
        while(170<tp_dev.x[0] && tp_dev.x[0]<234 && 70<tp_dev.y[0] && tp_dev.y[0]<95) tp_dev.scan(0);
    }
    
    if(90<tp_dev.x[0] && tp_dev.x[0]<154 && 90<tp_dev.y[0] && tp_dev.y[0]<115)//Return
    {
        Start = 0;Receive_flag = 0;MODE = 10;
        Shoot_flag = 0;
        aaa = 1;
        Display_Flag = 1;
        t = 0;
        flag_flip = 0;
        tt = 0;
        while(90<tp_dev.x[0] && tp_dev.x[0]<154 && 100<tp_dev.y[0] && tp_dev.y[0]<125) tp_dev.scan(0);
    }
    
    Receive_flag = 1;
    if(MODE == 10) Receive_flag = 0;
    
    //ÏÔÊ¾½çÃæË¢ÐÂÒ»ÏÂ
    if(Display_Flag==3)
    {
        Display_Flag=0;
        LCD_Fill(0,0,240,320,BLACK);//ÏÔÊ¾ÇøÓòÌî³äºÚÉ«
        BACK_COLOR=BLUE;
        LCD_ShowString(90,70 ,50,12,24,"Start1");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(345, 395, 415, 429);
        LCD_ShowString(170,70 ,50,12,24,"Start2");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(345, 395, 415, 429);
        BACK_COLOR=GREEN;  //ºìÉ«±³¾°É« 
        LCD_ShowString(90,100 ,50,12,24,"Retrn");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(345, 395, 415, 429);
    }
}
void Mode3_Select_Display(void)
{
    //´¥ÆÁ°´Å¥
    tp_dev.scan(0);
    if(90<tp_dev.x[0] && tp_dev.x[0]<154 && 70<tp_dev.y[0] && tp_dev.y[0]<95 && Start==0)//Start
    {
        Start = 1;
        while(90<tp_dev.x[0] && tp_dev.x[0]<154 && 70<tp_dev.y[0] && tp_dev.y[0]<95) tp_dev.scan(0);
    }
    
    if(90<tp_dev.x[0] && tp_dev.x[0]<154 && 90<tp_dev.y[0] && tp_dev.y[0]<115)//Return
    {
        Start = 0;Receive_flag = 0;MODE = 10;
        Shoot_flag = 0;
        aaa = 1;
        Display_Flag = 1;
        flag_flip = 0;
        tt = 0;
        tt_chong_flag = 1;
        while(90<tp_dev.x[0] && tp_dev.x[0]<154 && 100<tp_dev.y[0] && tp_dev.y[0]<125) tp_dev.scan(0);
    }
    
    Receive_flag = 1;
    if(MODE == 10) Receive_flag = 0;
    
    if(Display_Flag==4)
    {
        Display_Flag=0;
        LCD_Fill(0,0,240,320,BLACK);//ÏÔÊ¾ÇøÓòÌî³äºÚÉ«
        //DEBUG_MODE  °´Å¥
        POINT_COLOR=WHITE;	//»­±ÊÑÕÉ«
        
        BACK_COLOR=BLUE;  //ºìÉ«±³¾°É« ?
        LCD_ShowString(90,70 ,50,12,24,"Start");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(345, 395, 415, 429);
        
        BACK_COLOR=GREEN;  //ºìÉ«±³¾°É« 
        LCD_ShowString(90,100 ,50,12,24,"Retrn");//´¥ÃþÑ¡Ôñ
        //LCD_DrawRectangle(345, 395, 415, 429);
    }
}

void Debug_Data_Display(void)
{
    BACK_COLOR = BLACK;
    POINT_COLOR=WHITE;	//»­±ÊÑÕÉ«
    LCD_DrawLine(0,200,240,200);//debug·Ö½çÏß     
    //Çò×ø±ê
    LCD_ShowString(30,240 ,500,12,12," Dis_Set: ");
    if(!Switch_flag) BACK_COLOR = RED;
    LCD_ShowNum(90,240,(int)Dis,3,12);
    BACK_COLOR = BLACK;
    
    LCD_ShowString(130,240 ,500,12,12," Ang_Set: ");
    if(Switch_flag) BACK_COLOR = RED;
    if(Angle<0) 
    {
        LCD_ShowChar(190,240,'-',12,0);
        LCD_ShowNum(200,240,(int)(- Angle),3,12);    
    }
    else {
        LCD_ShowChar(190,240,'+',12,0);
        LCD_ShowNum(200,240,(int)(Angle),3,12);
    }
    BACK_COLOR = BLACK;
    
    LCD_ShowString(30,260 ,500,12,12," X_Now: ");LCD_ShowNum(80,260,(int)X,3,12);
    LCD_ShowString(130,260 ,500,12,12," Dis_Now: ");
    if(Start == 1) LCD_ShowNum(190,260,(int)Dis_Cam,3,12);
    else if(Start == 2) LCD_ShowNum(190,260,(int)Dis_Chao,3,12);
    LCD_ShowString(30,280 ,500,12,12," Ang_H: ");LCD_ShowNum(80,280,angle_H,3,12);
    
    LCD_ShowString(130,280,500,12,12," Ang_V: ");LCD_ShowNum(180,280,angle_V,3,12);
    //¶æ»úPWMÖµ
    LCD_ShowString(30,300 ,500,12,12," PWM_X: ");LCD_ShowNum(80,300,PWM_X,4,12);
    LCD_ShowString(130,300 ,500,12,12," PWM_Y: ");LCD_ShowNum(180,300,PWM_Y,4,12);     
}

///ÅÚ¿Ú²âËÙ ³¬Éù²¨²â¾à ¼¤¹â²â¾à


