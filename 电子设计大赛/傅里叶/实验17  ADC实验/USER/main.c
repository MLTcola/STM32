#include "sys.h"
#include "math.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
/************************************************
 ALIENTEK 战舰STM32F103开发板实验17
 ADC 实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
/*频率接A2
输出傅立叶变换接A1*/




u16 dat[] = 
{110,111,112,113,114,115,117,118,119,120,121,122,123,124,125,126,127,129,130,131,132,133,134,135,136,137,138,140,141,142,143,144,145,146,147,148,149,150,152,153,154,155,156,157,158,159,160,161,162,164,165,166,167,168,169,170,171,172,173,175,176,177,178,179,180,181,182,183,184,185,187,188,189,190,191,192,193,194,195,196,198,199,200,201,202,203,204,205,206,207,208,210,211,212,213,214,215,216,217,218,219,220,222,223,224,225,226,227,228,229,230,231,233,234,235,236,237,238,239,240,241,242,243,245,246,247,248,249,110,111,112,113,114,115,117,118,119,120,121,122,123,124,125,126,127,129,130,131,132,133,134,135,136,137,138,140,141,142,143,144,145,146,147,148,149,150,152,153,154,155,156,157,158,159,160,161,162,164,165,166,167,168,169,170,171,172,173,175,176,177,178,179,180,181,182,183,184,185,187,188,189,190,191,192,193,194,195,196,198,199,200,201,202,203,204,205,206,207,208,210,211,212,213,214,215,216,217,218,219,220,222,223,224,225,226,227,228,229,230,231,233,234,235,236,237,238,239,240,241,242,243,245,246,247,248,249,110,111,112,113,114,115,117,118,119,120,121,122,123,124,125,126,127,129,130,131,132,133,134,135,136,137,138,140,141,142,143,144,145,146,147,148,149,150,152,153,154,155,156,157,158,159,160,161,162,164,165,166,167,168,169,170,171,172,173,175,176,177,178,179,180,181,182,183,184,185,187,188,189,190,191,192,193,194,195,196,198,199,200,201,202,203,204,205,206,207,208,210,211,212,213,214,215,216,217,218,219,220,222,223,224,225,226,227,228,229,230,231,233,234,235,236,237,238,239,240,241,242,243,245,246,247,248,249,110,111,112,113,114,115,117,118,119,120,121,122,123,124,125,126,127,129,130,131,132,133,134,135,136,137,138,140,141,142,143,144,145,146,147,148,149,150,152,153,154,155,156,157,158,159,160,161,162,164,165,166,167,168,169,170,171,172,173,175,176,177,178,179,180,181,182,183,184,185,187,188,189,190,191,192,193,194,195,196,198,199,200,201,202,203,204,205,206,207,208,210,211,212,213,214,215,216,217,218,219,220,222,223,224,225,226,227,228,229,230,231,233,234,235,236,237,238,239,240,241,242,243,245,246,247,248,249,};
u16 H_FunDat[ADC_SIZE];
u16 H_SpeDat[ADC_SIZE];
 signed short lX,lY;
 float X,Y,Mag;
 unsigned short i;
mycomplex DAT[ADC_SIZE] = {0};	
 void DrawUI(void);
 void test(void);



extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值	

float f;
 int main(void)
 {	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	 
	KEY_Init();						//按键初始化
 	LED_Init();			     	//LED端口初始化
	LCD_Init();			 	
 	Adc_Init();		  			//ADC初始化
	TIM3_Int_Init(71,99);//72分频，1MHz/30=33kHz
	//TIM4_Int_Init(29999,7199);//7200分频，10kHz/10000=1Hz
	 
	TIM5_Cap_Init(99,72-1);	//以1Mhz的频率计数 
	 
	//POINT_COLOR=RED;//设置字体为红色 
	
	LCD_Fill(0,200,480,800,BLACK);				//背景
	 POINT_COLOR=RED;
	 LCD_ShowString(0,20,200,16,16,"JP");	
	 LCD_ShowString(0,40,200,16,16,"ERJIE");	
	 LCD_ShowString(0,60,200,16,16,"SANJIE");	
	 LCD_ShowString(0,80,200,16,16,"SIJIE");	
	 LCD_ShowString(0,100,200,16,16,"WUJIE");	
	 
	while(1)
	{
		u16 adcx;
		float temp;	
		u32 temp2=0; 
		u8 flag=0;
		float time;
		delay_ms(2000);
		test();
		adcmax=dat[0];
		adcmin=dat[0];
		POINT_COLOR=RED;
			for(i=0; i<512; i++)
		{
			if(dat[i]>adcmax)
			{
				adcmax=dat[i];
			}
			if(dat[i]<adcmin)
			{
				adcmin=dat[i];
			}
		}
		printf("adcmax:%d",adcmax);
//		POINT_COLOR=RED;
//		adcx=Get_Adc_Average_fuzhi(ADC_Channel_2,100);
//		temp=(float)adcx*(3.3/4096);
//		LCD_ShowString(2,120,200,16,16,"FUZHI");
//		printf("fuzhi%f",temp);
//		temp=temp*1000;
//		LCD_ShowxNum(50,120,temp,9,16,0);
//		temp=(int)temp*100%1000;
//		LCD_ShowString(130,120,200,16,16,".");
//		LCD_ShowxNum(135,120,temp,9,16,0);
//		LCD_ShowString(250,120,200,16,16,"mv");
//		TIM_Cmd(TIM5, ENABLE);
//		printf("adcmax:%d\n",adcmax);
//		for(i=0;i<100;i++)
//	{
//		if(adcmax<=Get_Adc2(ADC_Channel_2))
//		{
//			TIM_Cmd(TIM5, ENABLE);
//			flag=1;
//			continue;
//		}
//			if(adcmax<=Get_Adc2(ADC_Channel_2)&&(flag==1))
//		{
//			printf("num:%d",num);
//			TIM_Cmd(TIM5, DISABLE);
//			flag=0;
//			time=(float)10000/num;
//			printf("pinlv:%f",time);
//			LCD_ShowxNum(0,140,time,9,16,0);
//			num=0;
//			break;
//		}
// }
//	printf("141232\n");
	}
}
 
void test(void)
{int min,j;	
	LCD_ShowxNum(0,0,KEY_Scan(1),4,16,0);//按键测试

	if(KEY_Scan(1))LED0 = 0;						 //LED测试
	else LED0 = 1;
	if(KEY_Scan(1))
	{
		TIM_Cmd(TIM3, ENABLE);  //使能TIMx
	}		//ADC
	if(ADC_Flag==1)
	{
		ADC_Flag = 0;
		//LCD_Clear(DARKBLUE);
		LCD_DrawFunction(0,300,H_FunDat,480,200,BLACK);	//擦除
		//LCD_DrawSpectrum(0,550,H_SpeDat,480,200,BLACK);	//擦除
		LCD_DrawFunction(0,550,H_SpeDat,480,200,BLACK);	//擦除
		
		LCD_DrawGrid		(0,550,480,12,200,8,GRAY);	//绘制网格
		LCD_DrawGrid		(0,300,480,12,200,8,GRAY);	//绘制网格
		
		for(i=0; i<512; i++)
		{
			dat[i] = ADC_DAT_u32[i].real;
			H_FunDat[i] = dat[i]; 
//			printf("Fun=%d\n",H_FunDat[i]);
		} 
		LCD_DrawFunction(0,300,H_FunDat,480,200,YELLOW);	//绘制函数
		
		LCD_DrawGrid		(0,300,480,12,200,8,GRAY);	//绘制网格
		
		fft(512,ADC_DAT_u32); 
		  
		for(i=0; i<512; i++)
		{
			dat[i] = (u16)(ADC_DAT_u32[i].real+50000)/30;
			H_SpeDat[i] = dat[i];
			
			 lX  = ADC_DAT_u32[i].real;
        lY  = ADC_DAT_u32[i].imag;
        X = 512 * ((float)lX) / 32768;
        Y = 512 * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y) / 512;
        if(i == 0)
            dat[i] = (unsigned long)(Mag * 32768);
        else
            dat[i] = (unsigned long)(Mag * 65536);
//			printf("Spe=%f,%d\n",ADC_DAT_u32[i].real,H_SpeDat[i]);
//			printf("zhi=%d\n",dat[i]);
		}
		for(i=5;i<512/2;i++)
		{
			
			if(H_SpeDat[i-5]>H_SpeDat[i]&&H_SpeDat[i-4]>H_SpeDat[i]
				&&H_SpeDat[i-3]>H_SpeDat[i]&&H_SpeDat[i-2]>H_SpeDat[i]
				&&H_SpeDat[i]<H_SpeDat[i+2]&&H_SpeDat[i-1]>H_SpeDat[i]
			&&H_SpeDat[i]<H_SpeDat[i+1]&&H_SpeDat[i]<H_SpeDat[i+3]
			&&H_SpeDat[i]<H_SpeDat[i+4]&&H_SpeDat[i]<H_SpeDat[i+5])
			{
			j=i;
				break;
			}
		}
		POINT_COLOR=BLUE;
		LCD_ShowxNum(200,20,(float)j*19.53125,4,16,0);
		LCD_ShowxNum(300,20,ADC_DAT_u32[j].real/120,4,16,0);
			LCD_ShowxNum(200,40,(float)2*j*3.9,4,16,0);
		LCD_ShowxNum(300,40,ADC_DAT_u32[2*j].real/120,4,16,0);
			LCD_ShowxNum(200,60,(float)3*j*3.9,4,16,0);
		LCD_ShowxNum(300,60,ADC_DAT_u32[3*j].real/120,4,16,0);
			LCD_ShowxNum(200,80,(float)4*j*3.9,4,16,0);
		LCD_ShowxNum(300,80,ADC_DAT_u32[4*j].real/120,4,16,0);
			LCD_ShowxNum(200,100,(float)5*j*3.9,4,16,0);
		LCD_ShowxNum(300,100,ADC_DAT_u32[5*j].real/120,4,16,0);
		//LCD_DrawSpectrum(0,550,H_SpeDat,480,200,YELLOW);	//绘制频谱
		LCD_DrawFunction(0,550,H_SpeDat,480,200,YELLOW);	//绘制频谱
		LCD_DrawGrid		(0,550,480,12,200,8,GRAY);	//绘制网格
		//LCD_ShowString(60,150,200,16,16,"ADC_CH0_VOL:0.000V");
		//LCD_ShowxNum(156,150,H_SpeDat[i],1,16,0);
		//TIM_Cmd(TIM3, ENABLE);  //使能TIMx
	}
}
