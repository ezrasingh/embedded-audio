#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>

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

// Facade interface for interacting with pins in an interrupt-safe way
namespace System
{
    enum class Pin
    {
        Indicator,
        Output
    };

    // Initializes system settings, including pin configuration and ADC setup
    void setup();

    // Initialize system with serial
    void setupWithSerial(unsigned long baud);

    // Turns off the specified pin
    void turnOff(Pin pin);

    // Turns on the specified pin
    void turnOn(Pin pin);

    // Interface with hardware converters
    byte readADC();
    void writeDAC(byte signal);
};

#endif // SYSTEM_H
