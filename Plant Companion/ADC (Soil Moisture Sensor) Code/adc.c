#include "stm32f10x.h"

void ADC_Init(void) {
    // 1. Enable clocks for GPIOA (Input) and ADC1
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_ADC1EN;
    
    // 2. Configure PA0 as Analog Input (MODE0=00, CNF0=00)
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);

    // 3. Set ADC clock prescaler (Must be < 14MHz; 72MHz / 6 = 12MHz)
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

    // 4. Configure ADC1: Single conversion, Channel 0 
    ADC1->SQR3 = 0; // Channel 0 is first in sequence
    ADC1->SMPR2 |= ADC_SMPR2_SMP0; // Set sample time for stability

    // 5. Wake up ADC and calibrate
    ADC1->CR2 |= ADC_CR2_ADON;
    for(volatile int i=0; i<1000; i++); // Power-up delay
    ADC1->CR2 |= ADC_CR2_CAL;
    while(ADC1->CR2 & ADC_CR2_CAL){
			__NOP();
		}
}

uint16_t ADC_Read(void) {
    ADC1->CR2 |= ADC_CR2_ADON;       // Start conversion
    while(!(ADC1->SR & ADC_SR_EOC)){
				__NOP();
		}// Wait for completion
    return (uint16_t)ADC1->DR;       // Return 12-bit value
}
