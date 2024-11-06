#ifndef PITCH_DETECTOR_H
#define PITCH_DETECTOR_H

#include "core/signal.h"
#include "detectors/base/detector.h"
#include "detectors/level/level.h"
#include "detectors/pattern/pattern.h"

// Pitch detector to integrate multiple signal analyzers
struct PitchDetector : public Detector
{
private:
    SignalAnalyzer *signal;
    LevelDetector *levelDetector;
    PatternDetector *patternDetector;

public:
    PitchDetector(SignalAnalyzer *analyzer, LevelDetector *levelDetector, PatternDetector *patternDetector)
        : signal(analyzer), levelDetector(levelDetector), patternDetector(patternDetector) {}

    void reset() override;
    void update() override;
    void detect(void (*pinWriter)());
};

#endif // PITCH_DETECTOR_H