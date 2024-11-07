# Embedded Audio IO Processing

This project implements a signal detection system designed for real-time audio signal processing using Arduino. It leverages multiple detectors to identify signal clipping, frequency, and other properties in an incoming audio stream. The system is modular, with various detection modules to handle different aspects of signal analysis.

## Example Code

The following is an example of how to integrate and use the system in your Arduino sketch:

```cpp
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

  // If the level detector goes above-threshold
  if (levelDetector.detect() == LevelDetector::Result::AboveThreshold)
  {
    Pitch pitch = pitchDetector.pitch(); // Get pitch from pattern detector
    Serial.println(pitch.note());        // Print note from pitch
  }

  delay(1000); // Delay for 1 second before the next loop iteration
}

DAC_LOOP( // passthrough
    System::writeDAC(signal.input());

    ) // DAC_LOOP


ADC_LOOP(
    System::turnOff(System::Pin::Output); // Turn off output pin initially

    signal.update(System::readADC());     // Update signal with the latest ADC value
    Patterns::Trigger trigger = signal.isTriggered(ADC_EQUILIBRIUM);

    if (trigger == Patterns::Trigger::PositiveEdge) {
      pitchDetector.detect([]() {            // Defer execution with closure
        System::turnOn(System::Pin::Output); // Detect pitch and turn on output pin
      });
    }
    if (signal.isClipping()) {                // If clipping is detected
      System::turnOn(System::Pin::Indicator); // Turn on the clipping indicator LED
    }
    // update detectors
    pitchDetector.update();
    levelDetector.update();

    ) // ADC_LOOP
```

## Detectors Overview

The detectors folder contains several detectors that analyze different aspects of the signal:

- **PitchDetector**: Detects the frequency of the incoming signal.
- **LevelDetector**: Detects whether the signal is above or below a set threshold.
- **SlopeDetector**: Detects the rate of change of the signal.
- **PitchDetector**: Detects the pitch of the incoming signal.

Each detector is implemented as a class with its own header and source files. The system is designed to be modular, so you can easily add or remove detectors based on your specific needs.

## Contributing

Contributions are welcome! Please see the [contributing guidelines](CONTRIBUTING.md) for more information.

## License

This project is licensed under the [Apache 2.0](LICENSE-APACHE) or [MIT License](LICENSE-MIT) (your choice).
