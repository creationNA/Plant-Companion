#ifndef __BH1750_H
#define __BH1750_H

#include "stm32f10x.h"
#include "I2C.h"

// Default I2C Address (0x23 if ADDR pin is grounded)
#define BH1750_ADDR 0x23 

void BH1750_Init(void);
uint16_t BH1750_ReadLux(void);

#endif
