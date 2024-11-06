# Detectors Module

The `detectors` folder contains various classes and functions that are responsible for detecting different signal patterns, levels, frequencies, and other key metrics in the system. These detectors are designed to analyze data from the signal input and process it in real-time, supporting a variety of detection strategies for different signal conditions.

## Overview

The `detectors` folder contains several files, each corresponding to a specific type of signal detector. Below is a breakdown of the detectors and their corresponding files:

| **Detector**          | **Description**                                                      | **Header File** | **Source File** |
| --------------------- | -------------------------------------------------------------------- | --------------- | --------------- |
| **LevelDetector**     | Detects whether the signal level is above or below a set threshold.  | `level.h`       | `level.cpp`     |
| **SlopeDetector**     | Detects the slope of the signal, analyzing its rate of change.       | `slope.h`       | `slope.cpp`     |
| **PatternDetector**   | Detects specific patterns in the signal (e.g., pitch or modulation). | `pattern.h`     | `pattern.cpp`   |
| **PitchDetector**     | Detects the frequency (pitch) of the incoming signal.                | `pitch.h`       | `pitch.cpp`     |
| **FrequencyDetector** | Detects and calculates the frequency of the incoming signal.         | `frequency.h`   | `frequency.cpp` |

## Purpose

The primary purpose of the detectors is to analyze incoming signal data, detect various signal conditions, and trigger actions based on those conditions. Each detector is modular and designed to work together with others in a signal processing pipeline.

## Usage

To use the detectors, you need to include the appropriate header files in your project. Below is an example of how to integrate the detectors into your main application:

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

void setup() {
    // Initialize necessary hardware, like ADC and pins
    System::setup();
}

void loop() {
    // Update detectors with new signal data
    pitchDetector.detect([] {
        System::turnOn(System::Pin::Output);
    });
    pitchDetector.update();

    if (signal.isClipping()) {
        System::turnOn(System::Pin::Indicator);
    }

    // Further actions based on detected patterns or levels
}
```

## Future Improvements

- **More detector types**: Expand the module to include more sophisticated signal detectors.
- **Optimization**: Enhance performance to minimize latency in real-time applications.
- **Calibration**: Implement automatic calibration for better threshold detection and signal processing accuracy.

## License

This project is licensed under the MIT License - see the [LICENSE](../../LICENSE) file for details.

## Acknowledgments

- Thanks to the open-source community for providing inspiration and resources that helped in the development of the signal detection algorithms.
