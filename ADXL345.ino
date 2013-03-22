/* ******************************************************* */
/* I2C code for ADXL345 vAccelerometer                     */
/*                                                         */
/* ******************************************************* */
#include <Wire.h>

#include "AHRS.h"
#include "ADXL345.h"

//I2C addresses 
static uint8_t AccelAddress = 0x53; // Write:0xA6  Read:0xA7

//============================================
// Accelerometer
//============================================
void InitADXL345(void) {
    Wire.beginTransmission(AccelAddress);
    Wire.write((uint8_t)0x2D);  // power register
    Wire.write((uint8_t)0x08);  // measurement mode
    Wire.endTransmission();
    delay(20);
    Wire.beginTransmission(AccelAddress);
    Wire.write((uint8_t)0x31);  // Data format register
    Wire.write((uint8_t)0x08);  // set to full resolution
    Wire.endTransmission();
    delay(20);	
    // Because our main loop runs at 50Hz we adjust the output data rate to 50Hz (25Hz bandwith)
    // Wire.beginTransmission(AccelAddress);
    // Wire.write((uint8_t)0x2C);  // Rate
    // Wire.write((uint8_t)0x09);  // set to 50Hz, normal operation
    // Wire.endTransmission();
}


// Reads x,y and z vAccelerometer registers
void ReadADXL345(void) {
    int8_t i = 0;
    uint16_t buff[6];

    Wire.beginTransmission(AccelAddress); 
    Wire.write((uint8_t)0x32);        //sends address to read from
    Wire.endTransmission(); //end transmission

    Wire.beginTransmission(AccelAddress); //start transmission to device
    Wire.requestFrom(AccelAddress, (uint8_t)6);    // request 6 bytes from device

    while (Wire.available()) {  // ((Wire.available())&&(i<6))
        buff[i] = (uint16_t)Wire.read();  // receive one byte
        i++;
    }
    Wire.endTransmission(); //end transmission
    if (i == 6) { // All bytes received?
        //get the raw data
        vAccelRaw.x = (float)((buff[1]) << 8) | buff[0]); // X axis
        vAccelRaw.y = (float)((buff[3]) << 8) | buff[2]); // Y axis 
        vAccelRaw.z = (float)((buff[5]) << 8) | buff[4]); // Z axis
    } else {
        Serial.println("!ERR: Error reading accelerometer info!");
    }
}



