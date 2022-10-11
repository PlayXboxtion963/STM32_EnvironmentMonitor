#ifndef _dht11_init_H
#define _dht11_init_H
#include "GPIO.h"

int32_t Dht11_Start(void);
uint8_t Dht11_Read_Byte(void);
int32_t Dht11_Read(u8 *data);
#endif