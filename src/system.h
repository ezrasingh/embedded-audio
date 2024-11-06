#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>

// Interrupt Service Routine (ISR) when a new ADC value is ready
#define REGISTER_ADC_HANDLERS(actions) \
    ISR(ADC_vect)                      \
    {                                  \
        actions                        \
    }

// Facade interface for interacting with pins in an interrupt-safe way
namespace System
{
    enum Pin
    {
        Indicator,
        Output
    };

    // Initializes system settings, including pin configuration and ADC setup
    void setup();

    // Turns off the specified pin
    void turnOff(Pin pin);

    // Turns on the specified pin
    void turnOn(Pin pin);
};

#endif
