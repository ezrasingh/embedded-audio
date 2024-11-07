#ifndef LEVEL_DETECTOR_H
#define LEVEL_DETECTOR_H

#include <Signal.h>
#include "../base/detector.h"

// Level detector to assess signal amplitude against a threshold
struct LevelDetector : public Detector
{
private:
    unsigned int clock;
    byte maxAmp;
    byte prevMax;
    Signal *signal;

public:
    explicit LevelDetector(Signal *signal) : signal(signal) {}

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
