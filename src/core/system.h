#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>

// Interrupt Service Routine (ISR) when a new ADC value is ready
#define MONITOR_ADC(handler) \
    ISR(ADC_vect)            \
    {                        \
        handler              \
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
};

#endif // SYSTEM_H
