#include "System.h"
#include "core/constants.h"

// Setup continuous sampling of A0 pin at 38.5kHz
inline void setupADC()
{
    // Clear ADCSRA and ADCSRB registers
    ADCSRA = 0;
    ADCSRB = 0;
    // Configure ADC settings
    ADMUX |= (1 << REFS0);                 // Set reference voltage
    ADMUX |= (1 << ADLAR);                 // Left-align the ADC value (8-bit read from ADCH)
    ADCSRA |= (1 << ADPS2) | (1 << ADPS0); // Set ADC clock with 32 prescaler (16MHz/32 = 500kHz)
    ADCSRA |= (1 << ADATE);                // Enable auto-triggering
    ADCSRA |= (1 << ADIE);                 // Enable interrupt when measurement is complete
    ADCSRA |= (1 << ADEN);                 // Enable ADC
    ADCSRA |= (1 << ADSC);                 // Start ADC measurements
}

// Setup timer0 interrupt for continuous output at 40kHz
inline void setupDAC()
{
    // Clear TCCR0A and TCCR0B registers
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;                                   // Initialize counter to 0
    OCR0A = REGISTER_INCREMENT(DAC_SAMPLE_RATE); // Set match register increments for DAC sample rate
    TCCR0A |= (1 << WGM01);                      // Enable CTC mode
    TCCR0B |= (1 << CS11);                       // Set CS11 bit for 8 prescaler
    TIMSK0 |= (1 << OCIE0A);                     // Enable timer compare interrupt
}

// Initializes system settings and pin configurations
inline void System::setup()
{
    // Set pins 0-7 as outputs for DAC
    DDRD = B11111111;
    // Set pins 12 and 13 as outputs
    DDRB |= B00110000;
    noInterrupts(); // Disable interrupts
    setupADC();     // Setup ADC interrupts
    setupDAC();     // Setup DAC interrupts
    interrupts();   // Enable interrupts
}

// Initializes system settings and enable serial monitoring
void System::setupWithSerial(unsigned long baud)
{
    Serial.begin(baud);
    System::setup();
}
