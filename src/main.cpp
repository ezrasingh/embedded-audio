#include <Arduino.h>
#include "./state.cpp"

static SignalAnalyzer signal;
static LevelDetector levelDetector(&signal);
static SlopeDetector slopeDetector(&signal);
static PatternDetector patternDetector(&signal, &slopeDetector);
static PitchDetector pitchDetector(&signal, &levelDetector, &patternDetector);

// Facade interface for interacting withs pins in a interrupt-safe way
namespace System
{
  enum Pin
  {
    Indicator,
    Output
  };

  static void setup()
  {
    pinMode(13, OUTPUT); // led clippingindicator pin
    pinMode(12, OUTPUT); // output pin

    cli(); // disable interrupts

    // set up continuous sampling of analog pin 0 at 38.5kHz

    // clear ADCSRA and ADCSRB registers
    ADCSRA = 0;
    ADCSRB = 0;

    ADMUX |= (1 << REFS0); // set reference voltage
    ADMUX |= (1 << ADLAR); // left align the ADC value- so we can read highest 8 bits from ADCH register only

    ADCSRA |= (1 << ADPS2) | (1 << ADPS0); // set ADC clock with 32 prescaler- 16mHz/32=500kHz
    ADCSRA |= (1 << ADATE);                // enabble auto trigger
    ADCSRA |= (1 << ADIE);                 // enable interrupts when measurement complete
    ADCSRA |= (1 << ADEN);                 // enable ADC
    ADCSRA |= (1 << ADSC);                 // start ADC measurements

    sei(); // enable interrupts
  }

  static void turnOff(Pin pin)
  {
    if (pin == Pin::Output)
      PORTB &= B11101111; // set pin 12 low
    else if (pin == Pin::Indicator)
      PORTB &= B11011111; // set pin 13 low - turn off clipping indicator led
  }

  static void turnOn(Pin pin)
  {
    if (pin == Pin::Output)
      PORTB |= B00010000; // set pin 12 high
    else if (pin == Pin::Indicator)
      PORTB |= B00100000; // set pin 13 high- turn on clipping indicator led
  }
};

void setup()
{
  Serial.begin(9600);
  System::setup();
}

ISR(ADC_vect)
{ // when new ADC value ready
  System::turnOff(System::Pin::Output);

  signal.update(ADCH);
  pitchDetector.detect([]
                       { System::turnOn(System::Pin::Output); });
  pitchDetector.update();

  if (signal.isClipping())
    System::turnOn(System::Pin::Indicator);
}

void loop()
{

  // if currently clipping
  if (signal.isClipping())
    System::turnOff(System::Pin::Indicator);

  if (levelDetector.detect() == LevelDetector::Result::AboveThreshold)
  {
    unsigned int freq = patternDetector.frequency();
    Serial.print(freq);
    Serial.println(" hz");
  }

  delay(100);
}