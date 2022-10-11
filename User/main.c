#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "picture.h"
#include "GPIO.h"
#include "dht11_init.h"
#include "waterdetect.h"
#include "touch.h"
#include "LCD1602.h"
#include "lightsensor.h"

/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

int main()
{
	u8 i=0;
	u16 color=0;
	u8 data[5];
	u8 lsens_value=0;
	u8 shuzi=223;
	char strDst[256] = {0};
	int a;
	int ix=0;
		u16 penColor = BLUE;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();
	USART1_Init(115200);
	TFTLCD_Init();			//LCD初始化
	initGPIO();
	ADC1_Init();
	//TP_Init();
	KEY_Init();
	FRONT_COLOR=BLACK;
	BEEP_Init();
	My_EXTI_Init();
	Read_Flash(0X08070000,9);
	LCD_INIT();		//LCD1602初始化
	Lsens_Init();
	Hwjs_Init();


	choosedetect();
	if(upordown==1){
			upordown=0;
			LCD_ShowString(200,380,tftlcd_data.width,tftlcd_data.height,24,"       ");
			LCD_ShowString(60,380,tftlcd_data.width,tftlcd_data.height,24,"______");
			}else{
			upordown=1;
			LCD_ShowString(60,380,tftlcd_data.width,tftlcd_data.height,24,"      ");
			LCD_ShowString(200,380,tftlcd_data.width,tftlcd_data.height,24,"_______");

			}
	//LCD_ShowString(10,120,tftlcd_data.width,tftlcd_data.height,24,"www.prechin.cn");
	if(Dht11_Start()==0){
		LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,32,"success");
		}else{
		LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,32,"fail");
		}
//	LCD_Fill(10,150,60,180,GRAY);
//	color=LCD_ReadPoint(20,160);
	
//	printf("color=%x\r\n",color);
	
	//LCD_ShowPicture(20,220,200,112,(u8 *)gImage_picture);
		LCD_ShowFontHZ(10,60,"湿度");
		//LCD_ShowString(10,60,tftlcd_data.width,tftlcd_data.height,24,"moisture:");
		LCD_ShowFontHZ(10,110,"温度");
		//LCD_ShowString(10,110,tftlcd_data.width,tftlcd_data.height,24,"temp:");
		LCD_ShowFontHZ(10,160,"水位");
		LCD_ShowFontHZ(10,210,"亮度");
		LCD_ShowString(210,60,tftlcd_data.width,tftlcd_data.height,32,"%");
		LCD_ShowString(210,110,tftlcd_data.width,tftlcd_data.height,32,"C");

	while(1)
	{	
		
		PGout(3)=0;
		Dht11_Start();
		if(Dht11_Read(data)==0){
			FRONT_COLOR=GREEN;
		LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,32,"Success  read");
		}else{
			FRONT_COLOR=RED;
		LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,32,"fail     read");
		}
		FRONT_COLOR=BLACK;
		shuzi=data[0];
		LCD_WRITE_StrDATA( "hum:",0,0 );		//向屏幕第一行第五列输出HELLO
		sprintf( strDst,"%d",shuzi);
		LCD_WRITE_StrDATA( strDst,0,4);		//向屏幕第一行第五列输出HELLO
			
		if(shuzi>Moisupscale&&shuzi!=255){
			alerthigh(1);
		}else if(shuzi<Moisdownscale){
			alertlow(1);
		}
		FRONT_COLOR=BLACK;
		LCD_ShowxNum(120,60,shuzi,5,32,0);
		
		shuzi=data[2];
		LCD_WRITE_StrDATA( "Tem:",1,0 );
		sprintf( strDst,"%d",shuzi);
		LCD_WRITE_StrDATA( strDst,1,4);
		if(shuzi>Tempupscale&&shuzi!=255){
			alerthigh(2);
		}else if(shuzi<Tempdownscale){
			alertlow(2);
		}
		FRONT_COLOR=BLACK;
		LCD_ShowxNum(120,110,shuzi,5,32,0);
//		for(ix=0;ix!=5;ix++){
//			printf("%d,%d ",data[ix],ix);
//		}

		shuzi  = 3300000/4096*ADCConvertedValue[0]/1000;	
		LCD_WRITE_StrDATA( "Wat:",1,8 );
		sprintf( strDst,"%d",shuzi);
		LCD_WRITE_StrDATA( strDst,1,12);
		if(shuzi>Waterupscale){
			alerthigh(3);
		}else if(shuzi<Waterdownscale){
			alertlow(3);
		}
		FRONT_COLOR=BLACK;
		LCD_ShowxNum(120,160,shuzi,5,32,0);
		shuzi  = 3300000/4096*ADCConvertedValue[1]/1000;
		lsens_value=Lsens_Get_Val();
		shuzi	=	lsens_value;
		LCD_WRITE_StrDATA( "Lig:",0,8 );
		sprintf( strDst,"%d",shuzi);
		LCD_WRITE_StrDATA( strDst,0,12);
		if(shuzi>Lightupscale){
			alerthigh(4);
		}else if(shuzi<Lightdownscale){
			alertlow(4);
		}
		FRONT_COLOR=BLACK;
		LCD_ShowxNum(120,210,shuzi,5,32,0);
		
		PGout(3)=1;
		//LCD_ShowString(10,120,tftlcd_data.width,tftlcd_data.height,24,string);
		GPIO_SetBits(GPIOG,GPIO_Pin_4); //输出高
		delay_ms(1000);
		GPIO_ResetBits(GPIOG,GPIO_Pin_4); //输出高
		delay_ms(1000);
		LCD_ShowString(10,410,tftlcd_data.width,tftlcd_data.height,24,"                               ");
		LCD_ShowString(10,420,tftlcd_data.width,tftlcd_data.height,24,"                               ");
		LCD_WRITE_CMD( 0x01 );
	}
}


