#ifndef PATTERN_DETECTOR_H
#define PATTERN_DETECTOR_H

#include "core/signal.h"
#include "detectors/base/detector.h"
#include "detectors/slope/slope.h"

// Pattern detector to recognize periodic signal patterns
struct PatternDetector : public Detector
{
private:
    unsigned int clock;
    byte index;
    byte delta[MEASUREMENT_WINDOW];
    unsigned int timer[MEASUREMENT_WINDOW];
    unsigned int period;
    byte lastMatch;
    SignalAnalyzer *signal;
    SlopeDetector *slopeDetector;

public:
    explicit PatternDetector(SignalAnalyzer *analyzer, SlopeDetector *slopeDetector)
        : signal(analyzer), slopeDetector(slopeDetector) {}

    enum class Result
    {
        Init,
        Found,
        None
    };

    void reset() override;
    void update() override;

    float frequency() const;
    Result process();
    Result detect(void (*pinWriter)());
};

#endif // PATTERN_DETECTOR_H