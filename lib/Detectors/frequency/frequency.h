#ifndef FREQUENCY_DETECTOR_H
#define FREQUENCY_DETECTOR_H

#include <Signal.h>
#include "../base/detector.h"

// Frequency detector to measure signal frequency
struct FrequencyDetector : public Detector
{
private:
    unsigned int clock;
    unsigned int period;
    Signal *signal;

public:
    explicit FrequencyDetector(Signal *signal) : signal(signal) {}

    void reset() override;
    void update() override;

    float frequency() const;
};

#endif // FREQUENCY_DETECTOR_H
