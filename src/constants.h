#ifndef CONSTANTS_H
#define CONSTANTS_H

// Sampling Rate
constexpr int SAMPLE_RATE = 38462; // 38.462kHz

// ADC Settings for 8-bit precision
constexpr int ADC_MAX = 1023;        // Maximum ADC value
constexpr int ADC_EQUILIBRIUM = 127; // Midpoint (2.5V), represented as 127
constexpr int ADC_MIN = 0;           // Minimum ADC value

// Signal Analysis Thresholds
constexpr int SLOPE_TOLERANCE = 3;      // Adjust slope tolerance if needed
constexpr int TIMER_TOLERANCE = 10;     // Adjust timer tolerance if needed
constexpr int AMPLITUDE_THRESHOLD = 30; // Raise if signal is very noisy

// Measurement Window Size
constexpr int MEASUREMENT_WINDOW = 10; // Number of measurements before state reset

#endif // CONSTANTS_H
