#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

// Sampling Rate
constexpr unsigned int SAMPLE_RATE = 38462; // 38.462kHz

// ADC Settings for 8-bit precision
constexpr byte ADC_MAX = 255;                  // Maximum (5V) ADC value represented as 255
constexpr byte ADC_EQUILIBRIUM = ADC_MAX >> 1; // Midpoint (2.5V), represented as 127
constexpr byte ADC_MIN = 0;                    // Minimum ADC value

// Signal Analysis Thresholds
constexpr int SLOPE_TOLERANCE = 3;      // Adjust slope tolerance if needed
constexpr int TIMER_TOLERANCE = 10;     // Adjust timer tolerance if needed
constexpr int AMPLITUDE_THRESHOLD = 30; // Raise if signal is very noisy

// Measurement Window Size
constexpr unsigned int MEASUREMENT_WINDOW = 10; // Number of measurements before state reset

#endif // CONSTANTS_H
