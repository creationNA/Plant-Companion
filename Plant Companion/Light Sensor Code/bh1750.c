#include "bh1750.h"                      
                                             
void BH1750_Init(void)                       
{                                            
    for(volatile int i=0; i<100000; i++);    // Blocking delay to ensure the sensor hardware has time to power up
                                            
    i2c_sendStart();                         //Send the I2C START condition to gain control of the bus
    i2c_sendAddrForWrite(BH1750_ADDR);       //Address the BH1750 (0x23) and set the R/W bit to 0 (Write mode)
    i2c_sendData(0x01);                      //Send the "Power On" command (0x01) to wake the sensor from sleep
    i2c_sendStop();                          //Send the I2C STOP condition to end this transaction
                                             
    i2c_sendStart();                         //Send a new I2C START condition to prepare for a configuration command
    i2c_sendAddrForWrite(BH1750_ADDR);       //Address the BH1750 again in Write mode
    i2c_sendData(0x10);                      //Set sensor to "Continuous High Resolution Mode" (1 lux increments)
    i2c_sendStop();                          //Send the I2C STOP condition to finish the initialization sequence
}                                            
                                           
uint16_t BH1750_ReadLux(void)                
{                                           
    uint8_t h, l;                            // Declare two 8-bit variables to hold the high and low bytes of raw data
                                          
    i2c_sendStart();                         // Send the I2C START condition to request a data transfer
    i2c_sendAddrForRead(BH1750_ADDR);        // Address the sensor (0x23) and set the R/W bit to 1 (Read mode)
    h = i2c_readData(1);                     // Read the first (MSB) byte and send an ACK to keep the bus active
    l = i2c_readData(0);                     // Read the second (LSB) byte and send a NACK to tell the sensor we are done
    i2c_sendStop();                          // Send the I2C STOP condition to release the communication bus
                                             // 
	
    //The raw 16-bit value is formed by shifting the high byte and ORing it with the low byte.
    //Per the BH1750 datasheet, this value must be divided by 1.2 to calculate the actual Lux.
    //In integer-only math, dividing by 1.2 is exactly the same as multiplying by 5 and then dividing by 6.
    return (uint16_t)(((h << 8) | l) * 5 / 6); // Combine the bytes, apply the scale factor, and return the result
}                                           