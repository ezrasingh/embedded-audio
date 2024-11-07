#include "system.h"

// Initializes system settings, including pin configuration and ADC setup
void System::setup()
{

    // set digital pins 0-7 as outputs for DAC
    for (int i = 0; i <= 7; i++)
    {
        pinMode(i, OUTPUT);
    }
    pinMode(13, OUTPUT); // LED clipping indicator pin
    pinMode(12, OUTPUT); // Output pin

    noInterrupts(); // disable interrupts

    // Set up continuous sampling of A0 pin at 38.5kHz

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

    // Set timer0 interrupt at 40kHz

    // Clear TCCR0A and TCCR0B registers
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0; // Initialize counter value to 0
    // Set compare match register for 40khz increments
    OCR0A = 49;              // = (16*10^6) / (40000*8) - 1 (must be <256)
    TCCR0A |= (1 << WGM01);  // Enable CTC mode
    TCCR0B |= (1 << CS11);   // Set CS11 bit for 8 prescaler
    TIMSK0 |= (1 << OCIE0A); // Enable timer compare interrupt

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