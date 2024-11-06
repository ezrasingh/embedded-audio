#include <Arduino.h>

#include "core/signal.h"
#include "core/system.h"
#include "detectors/level/level.h"
#include "detectors/slope/slope.h"
#include "detectors/pattern/pattern.h"
#include "detectors/pitch/pitch.h"

static SignalAnalyzer signal;
static LevelDetector levelDetector(&signal);
static SlopeDetector slopeDetector(&signal);
static PatternDetector patternDetector(&signal, &slopeDetector);
static PitchDetector pitchDetector(&signal, &levelDetector, &patternDetector);

// Setup function for initializing serial communication and the system
void setup()
{
  Serial.begin(9600);
  System::setup();
  pitchDetector.setFundamentalFreq(440.0f);
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
    const Pitch pitch = patternDetector.pitch(); // Get pitch from pattern detector
    Serial.println(pitch.note());                // Print note from pitch
  }

  delay(1000); // Delay for 1 second before the next loop iteration
}

// Register audio input listener
REGISTER_ADC_HANDLERS(
    System::turnOff(System::Pin::Output); // Turn off output pin initially

    signal.update(ADCH); // Update signal with the latest ADC value
    pitchDetector.detect([]
                         { System::turnOn(System::Pin::Output); }); // Detect pitch and turn on output pin

    // If clipping is detected, turn on the clipping indicator LED
    if (signal.isClipping())
        System::turnOn(System::Pin::Indicator);)