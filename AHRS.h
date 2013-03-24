/*
*/
#ifndef AHRS_H
#define AHRS_H

#include <stdint.h>

#include "Vector.h"


//========================================
// Output Data Configuration
#define PRINT_JSON              (0)
#define PRINT_LEVIL             (0)
#define PRINT_LEVILBARO         (0)

// #ifdef __cplusplus
// extern "C"{
// #endif

extern float deltaT;
extern Vector3 vGyro;
extern Vector3 vAccel;
extern Vector3 vMag;
extern Vector3 vGyroOffset;
extern float gyroOffsetTemp;
extern Vector3 vGyroVariance;

extern Vector3 vGyroRaw;
extern Vector3 vAccelRaw;
extern intVector magRaw;

extern float angleTurned;

extern Matrix3 R;
extern Matrix3 orientation;
extern Matrix3 magGain;

extern Matrix2 F;
extern Matrix2 P;
extern Vector2 x_hat;

extern float roll;
extern float pitch;
extern float headingAngle;
extern float gLoad;
extern float slipAngle;
extern float turnRate;
extern float rollRate;
extern float pitchRate;
extern float magStrength;
extern short baroTemp;
extern long baroPres;
extern uint8_t battery;
extern float voltage;
extern int  baroAlt;
extern int  verticalSpeed;
extern float gyroTemp;
extern unsigned int gyroTempRaw;

// #ifdef __cplusplus
// }
// #endif

#endif // AHRS_H
