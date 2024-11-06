#ifndef LEVEL_DETECTOR_H
#define LEVEL_DETECTOR_H

#include "core/signal.h"
#include "detectors/base/detector.h"

// Level detector to assess signal amplitude against a threshold
struct LevelDetector : public Detector
{
private:
    unsigned int clock;
    byte maxAmp;
    byte prevMax;
    SignalAnalyzer *signal;

public:
    explicit LevelDetector(SignalAnalyzer *analyzer) : signal(analyzer) {}

    enum class Result
    {
        AboveThreshold,
        BelowThreshold
    };

    void reset() override;
    void update() override;

    Result detect(byte threshold = AMPLITUDE_THRESHOLD) const;
};

#endif // LEVEL_DETECTOR_H