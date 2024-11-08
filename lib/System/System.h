#ifndef SYSTEM_H
#define SYSTEM_H

#include "macros.h"

constexpr double SYSTEM_DELAY = 1000;

// Facade interface for interacting with pins
namespace System
{
    enum class Pin
    {
        Indicator, // pin 13
        Output     // pin 12
    };

    // Initializes system settings, including pin configuration and ADC setup
    inline void setup();
    // Initialize system with serial
    void setupWithSerial(unsigned long baud);
    // main loop
    void loop(void (*handler)());

    // Turns off the specified pin
    void turnOff(Pin pin);
    // Turns on the specified pin
    void turnOn(Pin pin);

    // Interface with hardware converters
    byte readADC();
    void writeDAC(byte signal);
};

#endif // SYSTEM_H
