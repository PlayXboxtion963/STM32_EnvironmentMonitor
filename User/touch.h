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
	
	FLASH_Unlock();//����flash
	
	for(i=0;i<len;i++)
	{
		NEW_ParArry[i] = *(u16 *)(addr + i*4);    //��ȡflash��ַ�е�ֵ
	}
	
	FLASH_Lock();//����
	if(NEW_ParArry[8]==1001){
/*********** ���¸�ֵ������ ********/		
	Tempupscale=NEW_ParArry[0];
	Tempdownscale=NEW_ParArry[1];
	Moisupscale=NEW_ParArry[2];
	Moisdownscale=NEW_ParArry[3];
	Waterupscale=NEW_ParArry[4];
	Waterdownscale=NEW_ParArry[5];
	Lightupscale=NEW_ParArry[6];
	Lightdownscale=NEW_ParArry[7];
		}//���flash�Ѿ���д��
}

/*
���ܣ���flash��д������
������addr:��ַ  flashdata1:д�������  len:д�볤��
����ֵ��
 */
void FLASH_WriteByte(u32 addr ,int *flashdata1,uint8_t len)
{
	
		int i = 0;
	
		FLASH_Status FLASHstatus = FLASH_COMPLETE;
		FLASH_Unlock(); //����flash

		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);  //�����־λ

		FLASHstatus=FLASH_ErasePage(addr);   //����ָ����ַҳ
		
		for(i=0; i<len;i++)
		{
			FLASHstatus=FLASH_ProgramHalfWord(addr+i*4, flashdata1[i]);//��ָ���ĵ�ַд������
		}
		FLASH_Lock();//���� 
}

int choosedetect(){
	
  switch(choose){
	  case 1:LCD_ShowFontHZ(10,300,"ʪ�ȷ�Χ����");	choose++;break;//��ʾʪ�ȵ���
	  case 2:LCD_ShowFontHZ(10,300,"�¶ȷ�Χ����");            	choose++;break;//��ʾ�¶ȵ���
	  case 3:LCD_ShowFontHZ(10,300,"ˮλ��Χ����");   			choose++;break;//��ʾˮ����
	  case 4:LCD_ShowFontHZ(10,300,"���ȷ�Χ����");   					choose=1;break;//��ʾ�ƹ����
  }
  displayscale();
}
void displayscale(){

switch(choose){
	  case 2:LCD_ShowxNum(30,350,Moisdownscale,5,32,0);	  LCD_ShowxNum(180,350,Moisupscale,5,32,0);   ;break;//��ʾʪ�ȵ��ڷ�Χ
	  case 3:LCD_ShowxNum(30,350,Tempdownscale,5,32,0);	  LCD_ShowxNum(180,350,Tempupscale,5,32,0);           ;break;//��ʾ�¶ȵ���
	  case 4:LCD_ShowxNum(30,350, Waterdownscale,5,32,0);	  LCD_ShowxNum(180,350,Waterupscale,5,32,0); 			;break;//��ʾˮ����
	  case 1:LCD_ShowxNum(30,350, Lightdownscale,5,32,0);	  LCD_ShowxNum(180,350,Lightupscale,5,32,0); 			;break;//��ʾ�ƹ����
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
	  case 2:Moisupscale++;break;//��ʾʪ�ȵ��ڷ�Χ
	  case 3: Tempupscale++  ;break;//��ʾ�¶ȵ���
	  case 4:	Waterupscale++		;break;//��ʾˮ����
	  case 1:Lightupscale++	;break;//��ʾ�ƹ����
  }
	  displayscale();
}
void Upreducevar(){
switch(choose){
	  case 2:Moisupscale--;break;//��ʾʪ�ȵ��ڷ�Χ
	  case 3: Tempupscale--  ;break;//��ʾ�¶ȵ���
	  case 4:	Waterupscale--		;break;//��ʾˮ����
	  case 1:Lightupscale--;break;//��ʾ�ƹ����
  }
	  displayscale();
}
void downplusvar(){
switch(choose){
	  case 2:Moisdownscale++;break;//��ʾʪ�ȵ��ڷ�Χ
	  case 3: Tempdownscale++ ;break;//��ʾ�¶ȵ���
	  case 4:	 Waterdownscale++		;break;//��ʾˮ����
	  case 1:Lightdownscale++	;break;//��ʾ�ƹ����
  }
  displayscale();
}
void downreducevar(){
switch(choose){
	  case 2:Moisdownscale--;break;//��ʾʪ�ȵ��ڷ�Χ
	  case 3: Tempdownscale-- ;break;//��ʾ�¶ȵ���
	  case 4:	 Waterdownscale--		;break;//��ʾˮ����
	  case 1:Lightdownscale--	;break;//��ʾ�ƹ����
  }
  displayscale();
}



#define KEY0_PIN   			GPIO_Pin_4    	//����KEY0�ܽ�
#define KEY1_PIN    		GPIO_Pin_3    	//����KEY1�ܽ�
#define KEY2_PIN  			GPIO_Pin_2
#define KEY_UP_PIN  		GPIO_Pin_0  	//����KEY_UP�ܽ�

#define KEY_PORT 			GPIOE 		//����˿�
#define KEY_UP_PORT 		GPIOA 		//����˿�

//ʹ��λ��������
#define KEY_UP 	PAin(0)
#define KEY0 	PEin(4)
#define KEY1 	PEin(3)
#define KEY2 	PEin(2)
//�����������ֵ  
#define KEY_UP_PRESS 	1
#define KEY0_PRESS		2
#define KEY1_PRESS		3
 
void KEY_Init(void);
u8 KEY_Scan(u8 mode);

/*******************************************************************************
* �� �� ��         : KEY_Init
* ��������		   : ������ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=KEY_UP_PIN;	   //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//��������  
	GPIO_Init(KEY_UP_PORT,&GPIO_InitStructure);		  /* ��ʼ��GPIO */
	
	GPIO_InitStructure.GPIO_Pin=KEY0_PIN|KEY1_PIN|KEY2_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
	GPIO_Init(KEY_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* �� �� ��         : KEY_Scan
* ��������		   : ����ɨ����
* ��    ��         : mode=0:���ΰ��°���
					 mode=1���������°���
* ��    ��         : 0��δ�а�������
					 KEY_UP_PRESS��KEY_UP������
					 KEY0_PRESS��KEY0������
					 KEY1_PRESS��KEY1������
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	
	if(mode==1) //������������
		key=1;
	if(key==1&&(KEY_UP==1||KEY0==0||KEY1==0)) //����һ����������
	{
		delay_ms(10);  //����
		key=0;
		if(KEY_UP==1)
			return KEY_UP_PRESS; 
		else if(KEY0==0)
			return KEY0_PRESS; 
		else if(KEY1==0)
			return KEY1_PRESS; 
	}
	else if(KEY_UP==0&&KEY0==1&&KEY1==1)    //�ް�������
		key=1;
	return 0;
}
/*******************************************************************************
* �� �� ��         : My_EXTI_Init
* ��������		   : �ⲿ�жϳ�ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void My_EXTI_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);//ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);//ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);//ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//ѡ��GPIO�ܽ������ⲿ�ж���·
	
	
	//EXTI0 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���	
	
	//EXTI0 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//EXTI0�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���	
	
	//EXTI3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//EXTI3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//EXTI4 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//EXTI4�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
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
/*  ������ʱ�Ӷ˿ڡ����Ŷ��� */
#define BEEP_PORT 			GPIOB   
#define BEEP_PIN 			GPIO_Pin_8//Ӧ����8
#define BEEP_PORT_RCC		RCC_APB2Periph_GPIOB
#define BEEP PBout(8)

/*******************************************************************************
* �� �� ��         : EXTI0_IRQHandler
* ��������		   : �ⲿ�ж�0����
* ��    ��         : ��
* ��    ��         : ��
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
* �� �� ��         : EXTI3_IRQHandler
* ��������		   : �ⲿ�ж�3����
* ��    ��         : ��
* ��    ��         : ��
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
* �� �� ��         : EXTI4_IRQHandler
* ��������		   : �ⲿ�ж�4����
* ��    ��         : ��
* ��    ��         : ��
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
* �� �� ��         : BEEP_Init
* ��������		   : ��������ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void BEEP_Init(void)	  //�˿ڳ�ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ���ṹ�������������ʼ��GPIO

	RCC_APB2PeriphClockCmd(BEEP_PORT_RCC,ENABLE);   /* ����GPIOʱ�� */

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=BEEP_PIN;		//ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		  //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(BEEP_PORT,&GPIO_InitStructure); 	 /* ��ʼ��GPIO */
	
	GPIO_ResetBits(BEEP_PORT,BEEP_PIN);
}

void alerthigh(int reason){

switch(reason){
	

	case 1:LCD_ShowFontHZ(10,410,"ʪ�ȳ�������");;;break;
	case 2:LCD_ShowFontHZ(10,410,"�¶ȳ�������");;break;
	case 3:LCD_ShowFontHZ(10,410,"ˮλ��������");;break;
	case 4:LCD_ShowFontHZ(10,410,"���ȳ�������");;break;
} 
	PGout(8)=1;
	BEEP=1;
	delay_ms(1000);
	BEEP=0;
	PGout(8)=0;
}

void alertlow(int reason){

switch(reason){
	
	case 1:LCD_ShowFontHZ(10,410,"ʪ�ȵ�������");;break;
	case 2:LCD_ShowFontHZ(10,410,"�¶ȵ�������");;break;
	case 3:LCD_ShowFontHZ(10,410,"ˮλ��������");;break;
	case 4:LCD_ShowFontHZ(10,410,"���ȵ�������");;break;
	
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

u32 hw_jsm;	  //����һ��32λ���ݱ��������������
u8  hw_jsbz;  //����һ��8λ���ݵı���������ָʾ���ձ�־


/*******************************************************************************
* �� �� ��         : Hwjs_Init
* ��������		   : ����˿ڳ�ʼ������	  ʱ�Ӷ˿ڼ��ⲿ�жϳ�ʼ�� 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Hwjs_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//�������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9); //ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_ClearITPendingBit(EXTI_Line9);
	
	/* �����ⲿ�жϵ�ģʽ */ 
	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

	/* ����NVIC���� */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;   //��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	 //��Ӧ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //ʹ��
	NVIC_Init(&NVIC_InitStructure);

}

/*******************************************************************************
* �� �� ��         : HW_jssj
* ��������		   : �ߵ�ƽ����ʱ�䣬����¼��ʱ�䱣����t�з��أ�����һ�δ�Լ20us 
* ��    ��         : ��
* ��    ��         : t
*******************************************************************************/
u8 HW_jssj()
{
	u8 t=0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==1)//�ߵ�ƽ
	{
		t++;
		delay_us(20);
		if(t>=250) return t;//��ʱ���
	}
	return t;
}


void EXTI9_5_IRQHandler(void)	  //����ң���ⲿ�ж�
{
	u8 Tim=0,Ok=0,Data,Num=0;
	BEEP=1;
   while(1)
   {
	   	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==1)
		{
			 Tim=HW_jssj();//��ô˴θߵ�ƽʱ��

			 if(Tim>=250) break;//�������õ��ź�

			 if(Tim>=200 && Tim<250)
			 {
			 	Ok=1;//�յ���ʼ�ź�
			 }
			 else if(Tim>=60 && Tim<90)
			 {
			 	Data=1;//�յ����� 1
			 }
			 else if(Tim>=10 && Tim<50)
			 {
			 	Data=0;//�յ����� 0
			 }

			 if(Ok==1)
			 {
			 	hw_jsm<<=1;
				hw_jsm+=Data;

				if(Num>=32)
				{
					//LCD_ShowxNum(120,180,hw_jsm,10,24,0);
					code_analysis(hw_jsm);
					
					printf("��������� %0.8X\r\n",hw_jsm);
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
