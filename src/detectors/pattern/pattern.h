#ifndef PATTERN_DETECTOR_H
#define PATTERN_DETECTOR_H

#include "core/signal.h"
#include "detectors/base/detector.h"
#include "detectors/slope/slope.h"

// Pattern detector to recognize periodic signal patterns
struct PatternDetector : public Detector
{
private:
    unsigned int clock = 0;
    byte index = 0;
    byte delta[MEASUREMENT_WINDOW] = {};
    unsigned int timer[MEASUREMENT_WINDOW] = {};
    unsigned int period = 0;
    byte lastMatch = 0;
    SignalAnalyzer *signal;
    SlopeDetector *slopeDetector;

public:
    PatternDetector(SignalAnalyzer *analyzer, SlopeDetector *slopeDetector)
        : signal(analyzer), slopeDetector(slopeDetector) {}

    enum class Result
    {
        Init,
        Found,
        None
    };

    void reset() override;
    void update() override;
    unsigned int frequency() const;
    Result process();
    Result detect(void (*pinWriter)());
};

#endif // PATTERN_DETECTOR_H