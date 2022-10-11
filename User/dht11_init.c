#include "GPIO.h"
#include "dht11_init.h"

//����DHT11��������0 �쳣����-1����������ʱ��ͼ��д�����д��룩
int32_t Dht11_Start(void)
{
	u32 t = 0;

	//�����ź�
	Dht11_Pin_Mode(1);//1�����

	GPIO_SetBits(GPIOG,GPIO_Pin_5);
	delay_us(50);
	GPIO_ResetBits(GPIOG,GPIO_Pin_5);
	delay_ms(20);
	GPIO_SetBits(GPIOG,GPIO_Pin_5);
	delay_us(30);
	
	Dht11_Pin_Mode(0);
	
	t = 0;
	//�ȴ��͵�ƽ����
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) == 1)
	{
		t++;
		delay_us(2);
	
		if(t >= 1000) //�ȴ�2msδ�ܵȴ����͵�ƽ
		{
			return -1;
		}		
	}
	
	delay_us(40);
	t = 0;
	
	//�ȴ��ߵ�ƽ���� ���˵͵�ƽ
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) == 0)
	{
		t++;
		delay_us(2);
	
		if(t >= 80) //�ȴ�160usδ�ܵȴ����ߵ�ƽ
		{
			return -1;
		}		
	}		
	
	delay_us(40);
	t = 0;	
	//�ȴ��͵�ƽ���� ���˸ߵ�ƽ
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) == 1)
	{
		t++;
		delay_us(2);
	
		if(t >= 80) //�ȴ�160usδ�ܵȴ����͵�ƽ
		{
			return -1;
		}		
	}

	return 0;
}

//һ���Զ�ȡ��λ����
uint8_t Dht11_Read_Byte(void)
{
	u8 i, data = 0;  //0000 0000
	u32 t = 0;
	
	for(i=0; i<8; i++)
	{
		t = 0;	
		//�ȴ��ߵ�ƽ���� ���˵͵�ƽ
		while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) == 0)
		{
			t++;
			delay_us(2);
		
			if(t >= 50) //�ȴ�100usδ�ܵȴ����ߵ�ƽ
			{
				return 0;
			}	
		
		}		
	
		delay_us(40);
		
		if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) == 1)
		{
			data |= (1<<(7-i));
			t = 0;
			//�ȴ��͵�ƽ���� ���˸ߵ�ƽ
			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) == 1)
			{
				t++;
				delay_us(2);
			
				if(t >= 50) //�ȴ�100usδ�ܵȴ����͵�ƽ
				{
					return -1;
				}		
			}				
		}	
	}
	
	return data;	
}

//�ɹ�����0��ʧ�ܷ���-1
int32_t Dht11_Read(u8 *data)
{
	u8 i=0;
	
	for(i=0; i<5; i++)
	{
		data[i] = Dht11_Read_Byte();
	}
	
	if(data[4] == data[0]+data[1]+data[2]+data[3])
	{
		return 0;
		
	}
	else
	{
		return -1;	
	}

}

 

