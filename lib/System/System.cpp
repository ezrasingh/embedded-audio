#include "System.h"

// Turns off the specified pin
void System::turnOff(Pin pin)
{
    if (pin == Pin::Output)
        PORTB &= B11101111; // Set pin 12 low (turn off output pin)
    else if (pin == Pin::Indicator)
        PORTB &= B11011111; // Set pin 13 low (turn off clipping indicator LED)
}

// Turns on the specified pin
void System::turnOn(Pin pin)
{
    if (pin == Pin::Output)
        PORTB |= B00010000; // Set pin 12 high (turn on output pin)
    else if (pin == Pin::Indicator)
        PORTB |= B00100000; // Set pin 13 high (turn on clipping indicator LED)
}

// Reads hardware Analog->Digital value
byte System::readADC()
{
    return ADCH; // A0 pin
}

// Writes a sample to hardware Digital->Analog
void System::writeDAC(byte sample)
{
    PORTD = sample; // writes to pins 0-7
}
