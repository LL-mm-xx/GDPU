#ifndef _PID_H
#define _PID_H


/*******************************************************************************
//±àÂëµç»úPID¿ØÖÆ´úÂë
//×÷Õß£º»¯×÷³¾
//CSDN£º»¯×÷³¾
//ßÙÁ¨ßÙÁ¨£º»¯×÷³¾my
//ĞŞ¸ÄÈÕÆÚ:2021Äê4ÔÂ2ÈÕ16:33:58
//°æ±¾£ºV1.0								  
********************************************************************************
//V1.0 2021/4/2£	  
*******************************************************************************/
   	  
//Ç°×óÂÖ³õÊ¼»¯²ÎÊı
extern float P,I,D,Set_Temp;								//³õÊ¼»¯P,I,D£¬µ±Ç°Öµ£¬ÉèÖÃÖµ ³£Êı
extern float PID_OUT,PWM_Duty;    					//PIDÊä³ö           
extern float P_OUT,I_OUT,D_OUT;    					//±ÈÀıÊä³ö£¬»ı·ÖÊä³ö£¬Î¢·ÖÊä³ö
extern float Current_Error, Last_Error;    	//µ±Ç°Îó²î  ×îºóÎó²î
extern float Sum_Error;     			//Îó²î»ı·Ö 
extern float PID_I_MAX,PID_I_MIN,V_DATA_MAX,V_DATA_MIN;

//Ç°ÓÒÂÖ³õÊ¼»¯²ÎÊı
extern float Set_Temp1;											//³õÊ¼»¯P,I,D£¬µ±Ç°Öµ£¬ÉèÖÃÖµ ³£Êı
extern float PID_OUT1,PWM_Duty1;  				  //PIDÊä³ö           
extern float P_OUT1,I_OUT1,D_OUT1;  			  //±ÈÀıÊä³ö£¬»ı·ÖÊä³ö£¬Î¢·ÖÊä³ö
extern float Current_Error1, Last_Error1;   //µ±Ç°Îó²î  ×îºóÎó²î
extern float Sum_Error1;     		//Îó²î»ı·Ö 
extern float PID_I_MAX1,PID_I_MIN1,V_DATA_MAX1,V_DATA_MIN1;

//ºó×óÂÖ³õÊ¼»¯²ÎÊı
extern float Set_Temp2;											//³õÊ¼»¯P,I,D£¬µ±Ç°Öµ£¬ÉèÖÃÖµ ³£Êı
extern float PID_OUT2,PWM_Duty2;    				//PIDÊä³ö           
extern float P_OUT2,I_OUT1,D_OUT2;   				//±ÈÀıÊä³ö£¬»ı·ÖÊä³ö£¬Î¢·ÖÊä³ö
extern float Current_Error2, Last_Error2;   //µ±Ç°Îó²î  ×îºóÎó²î
extern float Sum_Error2;    	  //Îó²î»ı·Ö 
extern float PID_I_MAX2,PID_I_MIN2,V_DATA_MAX2,V_DATA_MIN2;

//ºóÓÒÂÖ³õÊ¼»¯²ÎÊı
extern float Set_Temp3;											//³õÊ¼»¯P,I,D£¬µ±Ç°Öµ£¬ÉèÖÃÖµ ³£Êı
extern float PID_OUT3,PWM_Duty3;   				  //PIDÊä³ö           
extern float P_OUT3,I_OUT3,D_OUT3;    			//±ÈÀıÊä³ö£¬»ı·ÖÊä³ö£¬Î¢·ÖÊä³ö
extern float Current_Error3, Last_Error3;   //µ±Ç°Îó²î  ×îºóÎó²î
extern float Sum_Error3;    	  //Îó²î»ı·Ö 
extern float PID_I_MAX3,PID_I_MIN3,V_DATA_MAX3,V_DATA_MIN3;

void PID_calc(float V_1, float V_2, float V_3, float V_4); //PIDËã·¨

#endif


