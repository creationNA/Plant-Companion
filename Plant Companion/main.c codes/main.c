#include "stm32f10x.h"
#include "LCD.h"
#include "GPIO.h"
#include "bh1750.h"
#include "adc.h"
#include <stdio.h>

// State Definitions
#define STATE_OK    0
#define STATE_WATER 1
#define STATE_LIGHT 2


#define AIR_VALUE    3500  
#define WATER_VALUE  500  


#define DRY_SECTION_START 2500 // Anything above 1734 is "Dry" 

// Light Threshold
#define LIGHT_LOW_THRESHOLD 500

int main(void) {
    // 1. Initialize Hardware
    LCD_Init();        
    LED_Init();        
    BH1750_Init();
    ADC_Init(); 
    
    int lastState = -1; 
    uint16_t currentLux = 0;
    uint16_t currentMoisture = 0;
    char displayBuffer[17]; 

    while(1) {
        // 2. Read Sensors
        currentLux = BH1750_ReadLux(); 
        currentMoisture = ADC_Read();  
        
        // 3. Calculate Moisture % using the Inverse Ratio 
        // Formula: ((AirValue - Current) / (AirValue - WaterValue)) * 100
        int moisturePercent = (AIR_VALUE - (int)currentMoisture) * 100 / (AIR_VALUE - WATER_VALUE);
        
        if (moisturePercent > 100) moisturePercent = 100;
        if (moisturePercent < 0)   moisturePercent = 0;

        // 4. Determine State (Priority: Water > Light > OK)
        int currentState;
        if (currentMoisture > DRY_SECTION_START) { 
            // Sensor is in the "Dry" range (above 1734) 
            currentState = STATE_WATER;
        } else if (currentLux < LIGHT_LOW_THRESHOLD) { 
            currentState = STATE_LIGHT;
        } else {
            currentState = STATE_OK; 
        }

        // 5. Update Display Row 1 & LEDs (Only on state change)
        if (currentState != lastState) {
            LCD_Clear(); 
            if (currentState == STATE_WATER) {
                LCD_SendString("WATER NOW!"); 
                Set_LED_Color('R'); // Red
            } else if (currentState == STATE_LIGHT) {
                LCD_SendString("LOW LIGHT"); 
                Set_LED_Color('B'); // Blue
            } else {
                LCD_SendString("PLANT: OK"); 
                Set_LED_Color('G'); // Green
            }
            lastState = currentState; 
        }
        
        // 6. Update Row 2 (Live Data)
        LCD_SendCommand(0xC0); // Move to second row
        sprintf(displayBuffer, "M:%d%% Lux:%d    ", moisturePercent, currentLux);
        LCD_SendString(displayBuffer);

        // 7. System Heartbeat Delay
        for(volatile int i=0; i<800000; i++);
    }
}