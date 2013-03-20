/*
*/
#ifndef APPLICATIONROUTINES_H
#define APPLICATIONROUTINES_H

#ifdef __cplusplus
extern "C"{
#endif

extern void DeviceCalibrate(void);
extern void ReadBattery(void);
extern void InitGyroAccel(void);
extern void ReadGyroAccel(void);
extern void GyroAccelCal(void);

#ifdef __cplusplus
}
#endif

#endif // APPLICATIONROUTINES_H
