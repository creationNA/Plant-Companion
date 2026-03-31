#include "LCD.h"

// 0x20 shifted left for 8-bit Write Address = 0x40
#define LCD_ADDR_WRITE 0x20

void LCD_Write_Internal(uint8_t data, uint8_t rs) {
    // Standard PCF8574 to LCD mapping:
    // P0=RS, P1=RW, P2=E, P3=Backlight, P4-P7=D4-D7
    uint8_t control = 0x08; // Backlight ON (Bit 3)
    if (rs) control |= 0x01; // RS ON (Bit 0)

    uint8_t high_nibble = (data & 0xF0) | control;
    uint8_t low_nibble = ((data << 4) & 0xF0) | control;

    // --- Send High Nibble ---
    i2c_sendStart();
    i2c_sendAddrForWrite(0x20); 
    
    i2c_sendData(high_nibble | 0x04); // E=1 (Bit 2)
    for(volatile int i=0; i<5000; i++); // Wait for LCD to "see" data
    i2c_sendData(high_nibble & ~0x04); // E=0
    for(volatile int i=0; i<5000; i++); // Wait for command to process
    i2c_sendStop();

    // --- Send Low Nibble ---
    i2c_sendStart();
    i2c_sendAddrForWrite(0x20);
    
    i2c_sendData(low_nibble | 0x04);  // E=1
    for(volatile int i=0; i<5000; i++); 
    i2c_sendData(low_nibble & ~0x04);  // E=0
    for(volatile int i=0; i<5000; i++); 
    i2c_sendStop();
}

void LCD_SendCommand(uint8_t cmd) { 
		LCD_Write_Internal(cmd, 0); 
}
void LCD_SendData(uint8_t data){ 
		LCD_Write_Internal(data, 1); 
}

void LCD_Init(void) {
    i2c_init();   // cite: 3
    i2c_enable(); // cite: 3
    
    // 1. Mandatory Wait for LCD Power-up
    for(int i=0; i<1000000; i++); 

    // 2. The "Wake Up" Sequence (Must be sent exactly like this)
    LCD_SendCommand(0x33); 
    for(int i=0; i<100000; i++); 
    LCD_SendCommand(0x32); // Switch to 4-bit mode
    
    // 3. Normal Configuration
    LCD_SendCommand(0x28); // 2 lines, 5x7 matrix
    LCD_SendCommand(0x0C); // Display ON, Cursor OFF
    LCD_SendCommand(0x06); // Increment cursor
    LCD_Clear();
}
void LCD_Clear(void) { LCD_SendCommand(0x01); }

void LCD_SendString(char *str) {
    while (*str) LCD_SendData(*str++);
}
