#ifndef FREQUENCY_DETECTOR_H
#define FREQUENCY_DETECTOR_H

#include "core/signal.h"
#include "detectors/base/detector.h"

// Frequency detector to measure signal frequency
struct FrequencyDetector : public Detector
{
private:
    unsigned int clock;
    unsigned int period;
    SignalAnalyzer *signal;

public:
    explicit FrequencyDetector(SignalAnalyzer *analyzer) : signal(analyzer) {}

    void reset() override;
    void update() override;

    float frequency() const;
};

#endif // FREQUENCY_DETECTOR_H