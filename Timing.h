/*
*/
#ifndef TIMING_H
#define TIMING_H


extern volatile unsigned long timer0_millis;

extern unsigned long DIYmillis(void);
extern void DIYdelay(unsigned long ms);

#endif // TIMING_H