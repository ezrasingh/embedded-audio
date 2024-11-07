#ifndef SYSTEM_MACROS_H
#define SYSTEM_MACROS_H

#include <Arduino.h>

// Computes compare match register increment from sample rate must be <=255
#define REGISTER_INCREMENT(SAMPLE_RATE) min(255, (F_CPU / (8 * SAMPLE_RATE)) - 1)

// Interrupt Service Routine (ISR) for monitoring ADC values
#define ADC_LOOP(handler) \
    ISR(ADC_vect)         \
    {                     \
        handler           \
    }

// Interrupt Service Routine (ISR) for streaming DAC values
#define DAC_LOOP(handler)  \
    ISR(TIMER0_COMPA_vect) \
    {                      \
        handler            \
    }

#endif // SYSTEM_MACROS_H
