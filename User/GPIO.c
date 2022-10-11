#include "GPIO.h"
//#include "stm32f10x_rcc.h"

GPIO_InitTypeDef GPIO_InitStructure;
void initGPIO(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_3| GPIO_Pin_5| GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8; //设定要操作的管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure); //根据设定参数初始化GPIOA
	GPIO_SetBits(GPIOG,GPIO_Pin_4); //输出高
	GPIO_SetBits(GPIOG,GPIO_Pin_3); //输出高
	GPIO_SetBits(GPIOG,GPIO_Pin_5); //输出高
	GPIO_ResetBits(GPIOG,GPIO_Pin_7); //输出高
	GPIO_ResetBits(GPIOG,GPIO_Pin_8); //输出高
}
void Dht11_Pin_Mode(int a){
	GPIO_InitTypeDef GPIO_InitStructure;
	if(a==1){
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //设定要操作的管脚
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置为推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
		GPIO_Init(GPIOG, &GPIO_InitStructure); //根据设定参数初始化GPIOA
		GPIO_SetBits(GPIOG,GPIO_Pin_5); //输出高
	}else{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //设定要操作的管脚
		GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU; //设置为输入
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
		GPIO_Init(GPIOG, &GPIO_InitStructure); //根据设定参数初始化GPIOA
		
	}
} 
