#include <Arduino.h>
#include "signal.h"
#include "detector/level.h"
#include "detector/slope.h"
#include "detector/pattern.h"
#include "detector/pitch.h"

static SignalAnalyzer signal;
static LevelDetector levelDetector(&signal);
static SlopeDetector slopeDetector(&signal);
static PatternDetector patternDetector(&signal, &slopeDetector);
static PitchDetector pitchDetector(&signal, &levelDetector, &patternDetector);

// Facade interface for interacting with pins in an interrupt-safe way
namespace System
{
  enum Pin
  {
    Indicator,
    Output
  };

  // Initializes system settings, including pin configuration and ADC setup
  static void setup()
  {
    pinMode(13, OUTPUT); // LED clipping indicator pin
    pinMode(12, OUTPUT); // Output pin

    cli(); // Disable interrupts

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

    sei(); // Enable interrupts
  }

  // Turns off the specified pin
  static void turnOff(Pin pin)
  {
    if (pin == Pin::Output)
      PORTB &= B11101111; // Set pin 12 low (turn off output pin)
    else if (pin == Pin::Indicator)
      PORTB &= B11011111; // Set pin 13 low (turn off clipping indicator LED)
  }

  // Turns on the specified pin
  static void turnOn(Pin pin)
  {
    if (pin == Pin::Output)
      PORTB |= B00010000; // Set pin 12 high (turn on output pin)
    else if (pin == Pin::Indicator)
      PORTB |= B00100000; // Set pin 13 high (turn on clipping indicator LED)
  }
};

// Setup function for initializing serial communication and the system
void setup()
{
  Serial.begin(9600);
  System::setup();
}

// Interrupt Service Routine (ISR) when a new ADC value is ready
ISR(ADC_vect)
{
  System::turnOff(System::Pin::Output); // Turn off output pin initially

  signal.update(ADCH); // Update signal with the latest ADC value
  pitchDetector.detect([]
                       { System::turnOn(System::Pin::Output); }); // Detect pitch and turn on output pin
  pitchDetector.update();                                         // Update pitch detector state

  // If clipping is detected, turn on the clipping indicator LED
  if (signal.isClipping())
    System::turnOn(System::Pin::Indicator);
}

// Main loop that checks for signal clipping and frequency detection
void loop()
{
  // If clipping is detected, ensure the clipping indicator LED is off
  if (signal.isClipping())
    System::turnOff(System::Pin::Indicator);

  // If the level detector detects an above-threshold condition, print the frequency
  if (levelDetector.detect() == LevelDetector::Result::AboveThreshold)
  {
    unsigned int freq = patternDetector.frequency(); // Get frequency from pattern detector
    Serial.print(freq);
    Serial.println(" Hz");
  }

  delay(1000); // Delay for 1 second before the next loop iteration
}
