/*
*/
// Released under Creative Commons License
// Based on code by Doug Weibel and Jose Julio which was based on ArduIMU v1.5 by Jordi Munoz
// and William Premerlani, Jose Julio and Doug Weibel
// Heavily rewritten by Alec Myers

// Axis definition:
// X axis pointing forward (AWAY FROM the battery connector)
// Y axis pointing to the right
// Z axis pointing down
// Positive pitch : nose up
// Positive roll : right wing down
// Positive yaw : clockwise

/* APM 2.0 Hardware
 ATMega2560@5V w/ external 16MHz resonator
 HMC5883L: Magnetometer
 MPU6000 Accelerometer+Gyro
 MS5611-01BA03: Barometric Pressure Sensor
 Mediatek 3329: GPS
 Programmer : 5.0v FTDI
 Arduino IDE : Select board  ""
 */
#include <EEPROM.h>
#include <math.h>
#include <Wire.h>

#include "AHRS.h"
#include "ApplicationRoutines.h"
#include "BMP085.h"
#include "DCM.h"
#include "HMC5883L.h"
#include "Output.h"
#include "Timing.h"


#define DEGTORAD        (0.01745329252) /* *pi/180 */
#define RADTODEG        (57.2957795131) /* *180/pi */
#define FAST            (1)
#define SLOW            (0)

#define SLEW_RATE       (0.004)

// S.D. of compass measurement 0.1 radians
#define SIGMA_angle     (0.1)
// don't apply g angle correction when g-load closer to 1 than this:
#define G_DEADBAND      (0.05)
// no of samples for initial calibration of gyro drift
#define NUM_SAMPLES     (1000)
// exponential decay for turn rate, etc
#define BETA            (0.975)
// decay for vertical speed
#define BETA2           (0.8)


#define SERIAL_MUX      (7)
#define RED_LED         (5)
#define STATUS_LED      (6)
// not installed on v3 board?
#define YELLOW_LED      (7)


float deltaT;    // Integration time (DCM algorithm)
static long timer = 0;   // general purpuse timer

// static long timer24 = 0; //Second timer used to print values

Vector3 vGyro;
Vector3 vAccel;
Vector3 vMag;
Vector3 vGyroOffset;
float gyroOffsetTemp = 0.0; // the die temp when the vGyroscope zero-offset was measured
Vector3 vGyroVariance;

Vector3 vGyroRaw;
Vector3 vAccelRaw;
intVector magRaw;

//for calibrating gyros:
float angleTurned;

Matrix3 R;             // the DCM
Matrix3 orientation;   // sensor platform -> body orientation
Matrix3 magGain;       // hard and soft iron compensation

//Kalman variables
Matrix2 F;             // Transfer function
Matrix2 P;             // Covariance
Vector2 x_hat;         // state estimator

// Euler angles etc
float roll;
float pitch;
float headingAngle;
float gLoad;
float slipAngle;
float turnRate;
float rollRate;
float pitchRate;
float magStrength;
short baroTemp;
long baroPres;
int  battery = 100;
float voltage;
int  baroAlt; //remains 0.0 on ardIMU
int  verticalSpeed;
float gyroTemp = 0.0;
unsigned int gyroTempRaw;

void setup(void) {
    Serial.begin(115200);
    pinMode(STATUS_LED, OUTPUT);  // Status LED
    //pinMode(debugPin, OUTPUT);  // debug LED

    Serial.println();
    Serial.println("AHRS firmware rewritten by Alec Myers.");
    delay(50);
    Wire.begin();    //Init the I2C
    delay(200);
    Serial.println("Init Gyros and Accelerometers...");
    InitGyroAccel();
    InitCompass();
    Serial.println("Calibrating device parameters...");
    DeviceCalibrate();

    timer = millis();
    delay(100);
    Serial.println("Beginning operation.");
}

void loop() {
    // Main loop runs at 200Hz
    if ((DIYmillis() - timer) >= 5) {
        timerOld = timer;
        timer=DIYmillis();
        // Real time of loop run.
        deltaT = (timer - timerOld) / 1000.0;

        if(deltaT > 1) {
            //keeps dt from blowing up
            deltaT = 0;
        }

        powerOnCounter++;
        compassCounter++;
        baroCounter++;
        powerCounter++;
        printCounter++;
        LEDCounter++;

        ReadGyroAccel();

        // 20Hz
        if (compassCounter > 10) {
            compassCounter = 0;
            ReadCompass();
            CorrectHeading();
        }

        if (powerOnCounter < 400) { //2 seconds
            Update(FAST);
        } else {
            powerOnCounter = 600;
            Update(SLOW);
        }

        if (powerCounter > 200) {  //1 Hz
            powerCounter = 0;
            ReadBattery();
            PrintPower();
        }

        // Output every 8 iterations
        if (printCounter > 7) {
            printCounter = 0;
            PrintData();
        }

        if (LEDCounter >  49) {
            LEDCounter = 0;
            StatusLEDToggle();
        }
    }
}


