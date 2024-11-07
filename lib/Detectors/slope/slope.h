#ifndef SLOPE_DETECTOR_H
#define SLOPE_DETECTOR_H

#include <Signal.h>
#include "../base/detector.h"

// Slope detector to monitor changes in signal slope
struct SlopeDetector : public Detector
{
private:
    unsigned int clock;
    byte maxSlope;
    Signal *signal;

public:
    explicit SlopeDetector(Signal *signal) : signal(signal) {}

    enum class Result
    {
        AboveTolerance,
        WithinTolerance,
        BelowTolerance
    };

    void reset() override;
    void update() override;
    Result detect();
};

#endif // SLOPE_DETECTOR_H
