#ifndef SLOPE_DETECTOR_H
#define SLOPE_DETECTOR_H

#include "core/signal.h"
#include "detectors/base/detector.h"

// Slope detector to monitor changes in signal slope
struct SlopeDetector : public Detector
{
private:
    unsigned int clock;
    byte maxSlope;
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