#ifndef PITCH_DETECTOR_H
#define PITCH_DETECTOR_H

#include "../signal.h"
#include "detector.h"
#include "level.h"
#include "pattern.h"

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