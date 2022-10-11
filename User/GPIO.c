#include "GPIO.h"
//#include "stm32f10x_rcc.h"

GPIO_InitTypeDef GPIO_InitStructure;
void initGPIO(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_3| GPIO_Pin_5| GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8; //�趨Ҫ�����Ĺܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //����Ϊ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure); //�����趨������ʼ��GPIOA
	GPIO_SetBits(GPIOG,GPIO_Pin_4); //�����
	GPIO_SetBits(GPIOG,GPIO_Pin_3); //�����
	GPIO_SetBits(GPIOG,GPIO_Pin_5); //�����
	GPIO_ResetBits(GPIOG,GPIO_Pin_7); //�����
	GPIO_ResetBits(GPIOG,GPIO_Pin_8); //�����
}
void Dht11_Pin_Mode(int a){
	GPIO_InitTypeDef GPIO_InitStructure;
	if(a==1){
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //�趨Ҫ�����Ĺܽ�
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //����Ϊ�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOG, &GPIO_InitStructure); //�����趨������ʼ��GPIOA
		GPIO_SetBits(GPIOG,GPIO_Pin_5); //�����
	}else{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //�趨Ҫ�����Ĺܽ�
		GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU; //����Ϊ����
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOG, &GPIO_InitStructure); //�����趨������ʼ��GPIOA
		
	}
} 
