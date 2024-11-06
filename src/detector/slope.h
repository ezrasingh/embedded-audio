#ifndef SLOPE_DETECTOR_H
#define SLOPE_DETECTOR_H

#include "../signal.h"
#include "detector.h"

// Slope detector to monitor changes in signal slope
struct SlopeDetector : public Detector
{
private:
    unsigned int time = 0;
    byte maxSlope = 0;
    SignalAnalyzer *signal;

public:
    explicit SlopeDetector(SignalAnalyzer *analyzer) : signal(analyzer) {}

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