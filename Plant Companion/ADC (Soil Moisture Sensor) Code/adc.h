#ifndef ADC_H
#define ADC_H

#include "stm32f10x.h" // Device header for STM32F1 registers

/*
 Scaled Thresholds (Datasheet 10-bit values * 4 for STM32 12-bit)
  AirValue (Dry): 520 * 4 = 2080 
  WaterValue (Wet): 260 * 4 = 1040 
 */
//#define AIR_VALUE   2080 
//#define WATER_VALUE 1040

// Function prototypes
void ADC_Init(void);
uint16_t ADC_Read(void);

#endif
