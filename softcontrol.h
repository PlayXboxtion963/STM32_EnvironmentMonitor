#include "system.h"
#include "usart.h"
int flag1=0;
int flag2=0;
int flag3=0;
int flag4=0;
void code_analysis(u32 data){
	switch(data){
	
		case 16724175:printf("打开1灯");
		if(flag1==0){
			GPIO_SetBits(GPIOG,GPIO_Pin_4);
			flag1=1;
		}else{
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);
			flag1=0;
		}break;
		case 16718055:printf("打开2灯");
		if(flag2==0){
			GPIO_SetBits(GPIOG,GPIO_Pin_6);
			flag2=1;
		}else{
			GPIO_ResetBits(GPIOG,GPIO_Pin_6);
			flag2=0;
		}break;
		case 16743045:printf("打开3灯");
		if(flag3==0){
			GPIO_SetBits(GPIOG,GPIO_Pin_8);
			flag3=1;
		}else{
			GPIO_ResetBits(GPIOG,GPIO_Pin_8);
			flag3=0;
		} break;//输出高;break;
		case 16716015:printf("打开所有灯");GPIO_SetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8);;break;
		case 16726215:printf("关闭所有灯");GPIO_ResetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8); break;//输出高;break;
		case 16734885:if(flag4==0){
			GPIO_SetBits(GPIOG,GPIO_Pin_7);
			flag4=1;
		}else{
			GPIO_ResetBits(GPIOG,GPIO_Pin_7);
			flag4=0;
		};break;
		
	}

}

