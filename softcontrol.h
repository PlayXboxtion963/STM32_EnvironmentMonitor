#include "system.h"
#include "usart.h"
int flag1=0;
int flag2=0;
int flag3=0;
int flag4=0;
void code_analysis(u32 data){
	switch(data){
	
		case 16724175:printf("��1��");
		if(flag1==0){
			GPIO_SetBits(GPIOG,GPIO_Pin_4);
			flag1=1;
		}else{
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);
			flag1=0;
		}break;
		case 16718055:printf("��2��");
		if(flag2==0){
			GPIO_SetBits(GPIOG,GPIO_Pin_6);
			flag2=1;
		}else{
			GPIO_ResetBits(GPIOG,GPIO_Pin_6);
			flag2=0;
		}break;
		case 16743045:printf("��3��");
		if(flag3==0){
			GPIO_SetBits(GPIOG,GPIO_Pin_8);
			flag3=1;
		}else{
			GPIO_ResetBits(GPIOG,GPIO_Pin_8);
			flag3=0;
		} break;//�����;break;
		case 16716015:printf("�����е�");GPIO_SetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8);;break;
		case 16726215:printf("�ر����е�");GPIO_ResetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8); break;//�����;break;
		case 16734885:if(flag4==0){
			GPIO_SetBits(GPIOG,GPIO_Pin_7);
			flag4=1;
		}else{
			GPIO_ResetBits(GPIOG,GPIO_Pin_7);
			flag4=0;
		};break;
		
	}

}

