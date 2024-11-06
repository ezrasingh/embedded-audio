#ifndef SAMPLE_RATE
#define SAMPLE_RATE 38462 // 38.462kHz
#endif
// The incoming signal from A0 with 8 bit precision (0-255)
#ifndef ADC_MAX
#define ADC_MAX 1023
#endif

#ifndef ADC_EQUILIBRIUM
#define ADC_EQUILIBRIUM 127 // the midpoint (2.5V) will give a value of 127
#endif

#ifndef ADC_MIN
#define ADC_MIN 0
#endif

#ifndef SLOPE_TOLERANCE
#define SLOPE_TOLERANCE 3 // slope tolerance- adjust this if you need
#endif

#ifndef TIMER_TOLERANCE
#define TIMER_TOLERANCE 10 // timer tolerance- adjust this if you need
#endif

#ifndef AMPLITUDE_THRESHOLD
#define AMPLITUDE_THRESHOLD 30 // raise if you have a very noisy signal
#endif

#ifndef MEASUREMENT_WINDOW
#define MEASUREMENT_WINDOW 10 // how many measurements to do before resetting state
#endif