#include <Arduino.h>
#include "core/signal.h"
#include "core/system.h"
#include "detectors/level/level.h"
#include "detectors/slope/slope.h"
#include "detectors/pitch/pitch.h"

static SignalAnalyzer signal;
static LevelDetector levelDetector(&signal);
static SlopeDetector slopeDetector(&signal);
static PitchDetector pitchDetector(&signal, &slopeDetector);

void setup()
{
  System::setupWithSerial(9600);
  pitchDetector.setFundamentalFreq(440.0f);
}

// Main loop that checks for signal clipping and frequency detection
void loop()
{
  // If clipping was detected, ensure the clipping indicator LED is off
  if (signal.isClipping())
    System::turnOff(System::Pin::Indicator);

  // If the level detector detects an above-threshold condition
  if (levelDetector.detect() == LevelDetector::Result::AboveThreshold)
  {
    String note = pitchDetector.pitch().note();
    Serial.println(note);
  }

  delay(1000);
}

DAC_LOOP( // passthrough
    System::writeDAC(signal.input());

    ) // DAC_LOOP

ADC_LOOP(
    System::turnOff(System::Pin::Output); // Turn off output pin initially

    signal.update(System::readADC()); // Update signal with the latest ADC value
    Patterns::Trigger trigger = signal.isTriggered(ADC_EQUILIBRIUM);

    if (trigger == Patterns::Trigger::PositiveEdge) {
      pitchDetector.detect([]() {            // Defer execution with closure
        System::turnOn(System::Pin::Output); // Detect pitch and turn on output pin
      });
    }

    if (signal.isClipping()) {                // If clipping is detected
      System::turnOn(System::Pin::Indicator); // Turn on the clipping indicator LED
    }

    pitchDetector.update();
    levelDetector.update();

    ) // ADC_LOOP
