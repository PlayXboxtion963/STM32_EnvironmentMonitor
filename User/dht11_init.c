#include "GPIO.h"
#include "dht11_init.h"

//启动DHT11正常返回0 异常返回-1（根据启动时序图，写出下列代码）
int32_t Dht11_Start(void)
{
	u32 t = 0;

	//启动信号
	Dht11_Pin_Mode(1);//1是输出

	GPIO_SetBits(GPIOG,GPIO_Pin_5);
	delay_us(50);
	GPIO_ResetBits(GPIOG,GPIO_Pin_5);
	delay_ms(20);
	GPIO_SetBits(GPIOG,GPIO_Pin_5);
	delay_us(30);
	
	Dht11_Pin_Mode(0);
	
	t = 0;
	//等待低电平到来
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) == 1)
	{
		t++;
		delay_us(2);
	
		if(t >= 1000) //等待2ms未能等待到低电平
		{
			return -1;
		}		
	}
	
	delay_us(40);
	t = 0;
	
	//等待高电平到来 过滤低电平
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) == 0)
	{
		t++;
		delay_us(2);
	
		if(t >= 80) //等待160us未能等待到高电平
		{
			return -1;
		}		
	}		
	
	delay_us(40);
	t = 0;	
	//等待低电平到来 过滤高电平
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) == 1)
	{
		t++;
		delay_us(2);
	
		if(t >= 80) //等待160us未能等待到低电平
		{
			return -1;
		}		
	}

	return 0;
}

//一次性读取八位数据
uint8_t Dht11_Read_Byte(void)
{
	u8 i, data = 0;  //0000 0000
	u32 t = 0;
	
	for(i=0; i<8; i++)
	{
		t = 0;	
		//等待高电平到来 过滤低电平
		while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) == 0)
		{
			t++;
			delay_us(2);
		
			if(t >= 50) //等待100us未能等待到高电平
			{
				return 0;
			}	
		
		}		
	
		delay_us(40);
		
		if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) == 1)
		{
			data |= (1<<(7-i));
			t = 0;
			//等待低电平到来 过滤高电平
			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) == 1)
			{
				t++;
				delay_us(2);
			
				if(t >= 50) //等待100us未能等待到低电平
				{
					return -1;
				}		
			}				
		}	
	}
	
	return data;	
}

//成功返回0，失败返回-1
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

 

