#include "GPIO.h"

void LED_Init(void) {
    // 1. Enable Clock for GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 

    // 2. Configure PA8, PA9, PA10 as Output Push-Pull (50MHz)
    // CRH handles pins 8-15
    GPIOA->CRH &= ~0x00000FFF; // Clear bits for 8, 9, and 10
    GPIOA->CRH |=  0x00000333; // Set to Output Push-Pull, 50MHz
}

void Set_LED_Color(char color) {
    // Reset all status pins first
    GPIOA->ODR &= ~((1 << 8) | (1 << 9) | (1 << 10));

    if (color == 'R') GPIOA->ODR |= (1 << 8);  // Red (Water Now)
    if (color == 'G') GPIOA->ODR |= (1 << 9);  // Green (OK)
    if (color == 'B') GPIOA->ODR |= (1 << 10); // Blue (Low Light)
}
