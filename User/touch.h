#include "SysTick.h"
#include "tftlcd.h"
#include "usart.h"
#include "stdlib.h"
#include "math.h"
#include "usart.h"
#include "SysTick.h" 
#include "string.h" 
#include "led.h" 
#include "stm32f10x_flash.h"
int Tempupscale=50;
int Tempdownscale=20;
int Moisupscale=80;
int Moisdownscale=40;
int Waterupscale=200;
int Waterdownscale=0;
int Lightupscale=80;
int Lightdownscale=20;
void displayscale();
int choose=1;
int upordown=1;
int data[9];


void Read_Flash(uint32_t addr,uint8_t len)
{
	
	uint64_t NEW_ParArry[13] = {0};
	u8 i=0;
	
	uint64_t rdata = 0;
	
	FLASH_Unlock();//解锁flash
	
	for(i=0;i<len;i++)
	{
		NEW_ParArry[i] = *(u16 *)(addr + i*4);    //读取flash地址中的值
	}
	
	FLASH_Lock();//上锁
	if(NEW_ParArry[8]==1001){
/*********** 重新赋值给变量 ********/		
	Tempupscale=NEW_ParArry[0];
	Tempdownscale=NEW_ParArry[1];
	Moisupscale=NEW_ParArry[2];
	Moisdownscale=NEW_ParArry[3];
	Waterupscale=NEW_ParArry[4];
	Waterdownscale=NEW_ParArry[5];
	Lightupscale=NEW_ParArry[6];
	Lightdownscale=NEW_ParArry[7];
		}//如果flash已经被写过
}

/*
功能：向flash中写入数据
参数：addr:地址  flashdata1:写入的数据  len:写入长度
返回值：
 */
void FLASH_WriteByte(u32 addr ,int *flashdata1,uint8_t len)
{
	
		int i = 0;
	
		FLASH_Status FLASHstatus = FLASH_COMPLETE;
		FLASH_Unlock(); //解锁flash

		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);  //清除标志位

		FLASHstatus=FLASH_ErasePage(addr);   //擦除指定地址页
		
		for(i=0; i<len;i++)
		{
			FLASHstatus=FLASH_ProgramHalfWord(addr+i*4, flashdata1[i]);//从指定的地址写入数据
		}
		FLASH_Lock();//上锁 
}

int choosedetect(){
	
  switch(choose){
	  case 1:LCD_ShowFontHZ(10,300,"湿度范围调节");	choose++;break;//显示湿度调节
	  case 2:LCD_ShowFontHZ(10,300,"温度范围调节");            	choose++;break;//显示温度调节
	  case 3:LCD_ShowFontHZ(10,300,"水位范围调节");   			choose++;break;//显示水调节
	  case 4:LCD_ShowFontHZ(10,300,"亮度范围调节");   					choose=1;break;//显示灯光调节
  }
  displayscale();
}
void displayscale(){

switch(choose){
	  case 2:LCD_ShowxNum(30,350,Moisdownscale,5,32,0);	  LCD_ShowxNum(180,350,Moisupscale,5,32,0);   ;break;//显示湿度调节范围
	  case 3:LCD_ShowxNum(30,350,Tempdownscale,5,32,0);	  LCD_ShowxNum(180,350,Tempupscale,5,32,0);           ;break;//显示温度调节
	  case 4:LCD_ShowxNum(30,350, Waterdownscale,5,32,0);	  LCD_ShowxNum(180,350,Waterupscale,5,32,0); 			;break;//显示水调节
	  case 1:LCD_ShowxNum(30,350, Lightdownscale,5,32,0);	  LCD_ShowxNum(180,350,Lightupscale,5,32,0); 			;break;//显示灯光调节
  }
	data[0]=Tempupscale;
	data[1]=Tempdownscale;
	data[2]=Moisupscale;
	data[3]=Moisdownscale;
	data[4]=Waterupscale;
	data[5]=Waterdownscale;
	data[6]=Lightupscale;
	data[7]=Lightdownscale;
	data[8]=1001;
	FLASH_WriteByte(0X08070000,data,9);
}
void Upplusvar(){
switch(choose){
	  case 2:Moisupscale++;break;//显示湿度调节范围
	  case 3: Tempupscale++  ;break;//显示温度调节
	  case 4:	Waterupscale++		;break;//显示水调节
	  case 1:Lightupscale++	;break;//显示灯光调节
  }
	  displayscale();
}
void Upreducevar(){
switch(choose){
	  case 2:Moisupscale--;break;//显示湿度调节范围
	  case 3: Tempupscale--  ;break;//显示温度调节
	  case 4:	Waterupscale--		;break;//显示水调节
	  case 1:Lightupscale--;break;//显示灯光调节
  }
	  displayscale();
}
void downplusvar(){
switch(choose){
	  case 2:Moisdownscale++;break;//显示湿度调节范围
	  case 3: Tempdownscale++ ;break;//显示温度调节
	  case 4:	 Waterdownscale++		;break;//显示水调节
	  case 1:Lightdownscale++	;break;//显示灯光调节
  }
  displayscale();
}
void downreducevar(){
switch(choose){
	  case 2:Moisdownscale--;break;//显示湿度调节范围
	  case 3: Tempdownscale-- ;break;//显示温度调节
	  case 4:	 Waterdownscale--		;break;//显示水调节
	  case 1:Lightdownscale--	;break;//显示灯光调节
  }
  displayscale();
}



#define KEY0_PIN   			GPIO_Pin_4    	//定义KEY0管脚
#define KEY1_PIN    		GPIO_Pin_3    	//定义KEY1管脚
#define KEY2_PIN  			GPIO_Pin_2
#define KEY_UP_PIN  		GPIO_Pin_0  	//定义KEY_UP管脚

#define KEY_PORT 			GPIOE 		//定义端口
#define KEY_UP_PORT 		GPIOA 		//定义端口

//使用位操作定义
#define KEY_UP 	PAin(0)
#define KEY0 	PEin(4)
#define KEY1 	PEin(3)
#define KEY2 	PEin(2)
//定义各个按键值  
#define KEY_UP_PRESS 	1
#define KEY0_PRESS		2
#define KEY1_PRESS		3
 
void KEY_Init(void);
u8 KEY_Scan(u8 mode);

/*******************************************************************************
* 函 数 名         : KEY_Init
* 函数功能		   : 按键初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=KEY_UP_PIN;	   //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//下拉输入  
	GPIO_Init(KEY_UP_PORT,&GPIO_InitStructure);		  /* 初始化GPIO */
	
	GPIO_InitStructure.GPIO_Pin=KEY0_PIN|KEY1_PIN|KEY2_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(KEY_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : KEY_Scan
* 函数功能		   : 按键扫描检测
* 输    入         : mode=0:单次按下按键
					 mode=1：连续按下按键
* 输    出         : 0：未有按键按下
					 KEY_UP_PRESS：KEY_UP键按下
					 KEY0_PRESS：KEY0键按下
					 KEY1_PRESS：KEY1键按下
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	
	if(mode==1) //连续按键按下
		key=1;
	if(key==1&&(KEY_UP==1||KEY0==0||KEY1==0)) //任意一个按键按下
	{
		delay_ms(10);  //消抖
		key=0;
		if(KEY_UP==1)
			return KEY_UP_PRESS; 
		else if(KEY0==0)
			return KEY0_PRESS; 
		else if(KEY1==0)
			return KEY1_PRESS; 
	}
	else if(KEY_UP==0&&KEY0==1&&KEY1==1)    //无按键按下
		key=1;
	return 0;
}
/*******************************************************************************
* 函 数 名         : My_EXTI_Init
* 函数功能		   : 外部中断初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void My_EXTI_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);//选择GPIO管脚用作外部中断线路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);//选择GPIO管脚用作外部中断线路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);//选择GPIO管脚用作外部中断线路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//选择GPIO管脚用作外部中断线路
	
	
	//EXTI0 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器	
	
	//EXTI0 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//EXTI0中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器	
	
	//EXTI3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//EXTI3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	//EXTI4 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//EXTI4中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line3|EXTI_Line4|EXTI_Line2; 
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}
/*  蜂鸣器时钟端口、引脚定义 */
#define BEEP_PORT 			GPIOB   
#define BEEP_PIN 			GPIO_Pin_8//应该是8
#define BEEP_PORT_RCC		RCC_APB2Periph_GPIOB
#define BEEP PBout(8)

/*******************************************************************************
* 函 数 名         : EXTI0_IRQHandler
* 函数功能		   : 外部中断0函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)==1)
	{
		delay_ms(10);
		if(KEY_UP==1)
		{
			if(upordown==1){
			upordown=0;
			LCD_ShowString(200,380,tftlcd_data.width,tftlcd_data.height,24,"       ");
			LCD_ShowString(60,380,tftlcd_data.width,tftlcd_data.height,24,"______");
			}else{
			upordown=1;
			LCD_ShowString(60,380,tftlcd_data.width,tftlcd_data.height,24,"      ");
			LCD_ShowString(200,380,tftlcd_data.width,tftlcd_data.height,24,"_______");

			}
			BEEP=!BEEP;
			delay_ms(10);
			BEEP=!BEEP;
			
		}	
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

/*******************************************************************************
* 函 数 名         : EXTI3_IRQHandler
* 函数功能		   : 外部中断3函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)==1)
	{
		delay_ms(10);
		if(KEY1==0)
		{	
			
			if(upordown==0){
			
			downplusvar();
			}else{
			
			Upplusvar();

			}
			LED2=!LED2;
			BEEP=!BEEP;
			delay_ms(10);
			BEEP=!BEEP;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

/*******************************************************************************
* 函 数 名         : EXTI4_IRQHandler
* 函数功能		   : 外部中断4函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)==1)
	{
		delay_ms(10);
		if(KEY0==0)
		{
			
			if(upordown==0){
			
			downreducevar();
			}else{
			
			Upreducevar();

			}
			LED1=!LED1;
			BEEP=!BEEP;
			delay_ms(10);
			BEEP=!BEEP;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)==1)
	{
		delay_ms(10);
		if(KEY2==0)
		{
			choosedetect();
			BEEP=!BEEP;
			delay_ms(10);
			BEEP=!BEEP;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/*******************************************************************************
* 函 数 名         : BEEP_Init
* 函数功能		   : 蜂鸣器初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void BEEP_Init(void)	  //端口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;	//声明一个结构体变量，用来初始化GPIO

	RCC_APB2PeriphClockCmd(BEEP_PORT_RCC,ENABLE);   /* 开启GPIO时钟 */

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=BEEP_PIN;		//选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		  //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(BEEP_PORT,&GPIO_InitStructure); 	 /* 初始化GPIO */
	
	GPIO_ResetBits(BEEP_PORT,BEEP_PIN);
}

void alerthigh(int reason){

switch(reason){
	

	case 1:LCD_ShowFontHZ(10,410,"湿度超过上限");;;break;
	case 2:LCD_ShowFontHZ(10,410,"温度超过上限");;break;
	case 3:LCD_ShowFontHZ(10,410,"水位超过上限");;break;
	case 4:LCD_ShowFontHZ(10,410,"亮度超过上限");;break;
} 
	PGout(8)=1;
	BEEP=1;
	delay_ms(1000);
	BEEP=0;
	PGout(8)=0;
}

void alertlow(int reason){

switch(reason){
	
	case 1:LCD_ShowFontHZ(10,410,"湿度低于下限");;break;
	case 2:LCD_ShowFontHZ(10,410,"温度低于下限");;break;
	case 3:LCD_ShowFontHZ(10,410,"水位低于下限");;break;
	case 4:LCD_ShowFontHZ(10,410,"亮度低于下限");;break;
	
}
	PGout(7)=1;
	BEEP=1;
	delay_ms(1000);
	BEEP=0;
	PGout(7)=0;
	
}
void code_analysis(u32 data){
	switch(data){
	
		case 16736925:choosedetect(); ;break;
		case 16720605:if(upordown==1){
			upordown=0;
			LCD_ShowString(200,380,tftlcd_data.width,tftlcd_data.height,24,"       ");
			LCD_ShowString(60,380,tftlcd_data.width,tftlcd_data.height,24,"______");
			}else{
			upordown=1;
			LCD_ShowString(60,380,tftlcd_data.width,tftlcd_data.height,24,"      ");
			LCD_ShowString(200,380,tftlcd_data.width,tftlcd_data.height,24,"_______");

			}
			
			
		;break;
		case 16712445:if(upordown==0){
			
			downreducevar();
			}else{
			
			Upreducevar();

			};break;
		case 16761405:if(upordown==0){
			
			downplusvar();
			}else{
			
			Upplusvar();

			};break;
		
	}

}

extern u32 hw_jsm;
extern u8  hw_jsbz;
extern u8  frame_cnt;

u32 hw_jsm;	  //定义一个32位数据变量，保存接收码
u8  hw_jsbz;  //定义一个8位数据的变量，用于指示接收标志


/*******************************************************************************
* 函 数 名         : Hwjs_Init
* 函数功能		   : 红外端口初始化函数	  时钟端口及外部中断初始化 
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Hwjs_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//红外接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9); //选择GPIO管脚用作外部中断线路
	EXTI_ClearITPendingBit(EXTI_Line9);
	
	/* 设置外部中断的模式 */ 
	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

	/* 设置NVIC参数 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;   //打开全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	 //响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //使能
	NVIC_Init(&NVIC_InitStructure);

}

/*******************************************************************************
* 函 数 名         : HW_jssj
* 函数功能		   : 高电平持续时间，将记录的时间保存在t中返回，其中一次大约20us 
* 输    入         : 无
* 输    出         : t
*******************************************************************************/
u8 HW_jssj()
{
	u8 t=0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==1)//高电平
	{
		t++;
		delay_us(20);
		if(t>=250) return t;//超时溢出
	}
	return t;
}


void EXTI9_5_IRQHandler(void)	  //红外遥控外部中断
{
	u8 Tim=0,Ok=0,Data,Num=0;
	BEEP=1;
   while(1)
   {
	   	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==1)
		{
			 Tim=HW_jssj();//获得此次高电平时间

			 if(Tim>=250) break;//不是有用的信号

			 if(Tim>=200 && Tim<250)
			 {
			 	Ok=1;//收到起始信号
			 }
			 else if(Tim>=60 && Tim<90)
			 {
			 	Data=1;//收到数据 1
			 }
			 else if(Tim>=10 && Tim<50)
			 {
			 	Data=0;//收到数据 0
			 }

			 if(Ok==1)
			 {
			 	hw_jsm<<=1;
				hw_jsm+=Data;

				if(Num>=32)
				{
					//LCD_ShowxNum(120,180,hw_jsm,10,24,0);
					code_analysis(hw_jsm);
					
					printf("红外接收码 %0.8X\r\n",hw_jsm);
					hw_jsbz=1;
					
				  	break;
				}
			 }

			 Num++;
		}
   }

   EXTI_ClearITPendingBit(EXTI_Line9);	
   BEEP=0;
}
