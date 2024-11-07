# Signal Detection System

This project implements a signal detection system designed for real-time audio signal processing using Arduino. It leverages multiple detectors to identify signal clipping, frequency, and other properties in an incoming audio stream. The system is modular, with various detection modules to handle different aspects of signal analysis.

## Example Code

The following is an example of how to integrate and use the system in your Arduino sketch:

```cpp
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
    Pitch pitch = pitchDetector.pitch(); // Get pitch from pattern detector
    Serial.println(pitch.note());        // Print note from pitch
  }

  delay(1000); // Delay for 1 second before the next loop iteration
}

MONITOR_ADC(
    System::turnOff(System::Pin::Output); // Turn off output pin initially
    signal.update(ADCH);                  // Update signal with the latest ADC value

    pitchDetector.detect([]() {            // Defer execution with closure
      System::turnOn(System::Pin::Output); // Detect pitch and turn on output pin
    });

    if (signal.isClipping())                // If clipping is detected
    System::turnOn(System::Pin::Indicator); // Turn on the clipping indicator LED
)
```

## Project Structure

The project consists of multiple modules for handling different aspects of signal processing. Below is a breakdown of the folder structure:

```shell
src/
├── core/
│   ├── signal.h          // Signal analyzer class and utilities
│   ├── system.h          // System interface for pin management and ADC setup
├── detectors/
│   ├── level/            // Detects signal levels above/below a threshold
│   ├── slope/            // Detects the slope of the signal (rate of change)
│   ├── pattern/          // Detects patterns in the signal
│   ├── pitch/            // Detects the pitch/frequency of the signal
└── main.cpp              // Main Arduino sketch

```

## Detectors Overview

The detectors folder contains several detectors that analyze different aspects of the signal:

- **LevelDetector**: Detects whether the signal is above or below a set threshold.
- **SlopeDetector**: Detects the rate of change of the signal.
- **PatternDetector**: Identifies specific patterns in the signal.
- **PitchDetector**: Detects the frequency (pitch) of the incoming signal.

Each detector is implemented as a class with its own header and source files. The system is designed to be modular, so you can easily add or remove detectors based on your specific needs.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
