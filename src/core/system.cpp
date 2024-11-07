#include "system.h"

// Initializes system settings, including pin configuration and ADC setup
void System::setup()
{
    pinMode(13, OUTPUT); // LED clipping indicator pin
    pinMode(12, OUTPUT); // Output pin
    noInterrupts();

    // Set up continuous sampling of analog pin 0 at 38.5kHz
    // Clear ADCSRA and ADCSRB registers
    ADCSRA = 0;
    ADCSRB = 0;

    // Configure ADC settings
    ADMUX |= (1 << REFS0); // Set reference voltage
    ADMUX |= (1 << ADLAR); // Left-align the ADC value (8-bit read from ADCH)

    ADCSRA |= (1 << ADPS2) | (1 << ADPS0); // Set ADC clock with 32 prescaler (16MHz/32 = 500kHz)
    ADCSRA |= (1 << ADATE);                // Enable auto-triggering
    ADCSRA |= (1 << ADIE);                 // Enable interrupt when measurement is complete
    ADCSRA |= (1 << ADEN);                 // Enable ADC
    ADCSRA |= (1 << ADSC);                 // Start ADC measurements

    interrupts(); // Enable interrupts
}

// Initializes system settings, including pin configuration and ADC setup
void System::setupWithSerial(unsigned long baud)
{
    Serial.begin(baud);
    System::setup();
}

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
