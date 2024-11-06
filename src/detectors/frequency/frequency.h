#ifndef FREQUENCY_DETECTOR_H
#define FREQUENCY_DETECTOR_H

#include "core/signal.h"
#include "detectors/base/detector.h"

// Frequency detector to measure signal frequency
struct FrequencyDetector : public Detector
{
private:
    unsigned int clock = 0;
    unsigned int period = 0;
    SignalAnalyzer *signal;

public:
    explicit FrequencyDetector(SignalAnalyzer *analyzer) : signal(analyzer) {}

    void reset() override;
    void update() override;
    unsigned int frequency() const;
};

#endif // FREQUENCY_DETECTOR_H