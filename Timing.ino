/*
*/
#include "Timing.h"


volatile unsigned long timer0_millis = 0;

// this function replaces the arduino millis() funcion
unsigned long DIYmillis(void) {
    // timer0_millis could change inside timer0 interrupt and we don´t want to disable interrupts 
    // we can do two readings and compare.
    unsigned long m = timer0_millis;
    unsigned long m2 = timer0_millis;

    // timer0_millis corrupted?
    if (m != m2) {              
        m = timer0_millis; // this should be fine...
    }

   return m;
}

void DIYdelay(unsigned long ms) {
    unsigned long start = DIYmillis();

    while (DIYmillis() - start <= ms) {
        /* */;
    }
}
