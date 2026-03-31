#ifndef __LCD_H
#define __LCD_H

#include "stm32f10x.h"
#include "I2C.h"

// Define the I2C address for the display

// Function Prototypes
void LCD_Init(void);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_SendString(char *str);
void LCD_Clear(void);

#endif
