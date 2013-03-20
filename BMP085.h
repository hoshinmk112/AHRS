/*
*/
#ifndef BMP085_H
#define BMP085_H

// #ifdef __cplusplus
// extern "C"{
// #endif

extern void InitBaro(void);
extern void ReadTemperature(void);
extern void ReadPressure(void);
extern void ReadAltitude(void);

// #ifdef __cplusplus
// }
// #endif

#endif // BMP085_H
