/*
*/
#ifndef TIMING_H
#define TIMING_H

#ifdef __cplusplus
extern "C"{
#endif

extern volatile unsigned long timer0_millis;

extern unsigned long DIYmillis(void);
extern void DIYdelay(unsigned long ms);

#ifdef __cplusplus
}
#endif

#endif // TIMING_H