#ifndef PITCH_DETECTOR_H
#define PITCH_DETECTOR_H

#include "utils.h"
#include "core/signal.h"
#include "detectors/base/detector.h"
#include "detectors/level/level.h"
#include "detectors/pattern/pattern.h"

// Reference note information for 12-tone equal temperament
constexpr float DEFAULT_A4 = 440.0f;

// read only pitch data
struct Pitch
{
private:
    const uint8_t index;
    const float deviation;

public:
    Pitch(const uint8_t i, const float d) : index(i), deviation(d) {}

    float cents() const;
    String note() const;
};

// Pitch detector to integrate multiple signal analyzers
struct PitchDetector : public Detector
{
    float fundamentalFreq;

private:
    SignalAnalyzer *signal;
    LevelDetector *levelDetector;
    PatternDetector *patternDetector;

public:
    explicit PitchDetector(SignalAnalyzer *analyzer, LevelDetector *levelDetector, PatternDetector *patternDetector)
        : signal(analyzer),
          levelDetector(levelDetector),
          patternDetector(patternDetector),
          fundamentalFreq(DEFAULT_A4) {}

    void reset() override;
    void update() override;

    void detect(void (*pinWriter)());
    void setFundamentalFreq(float freq);
    Pitch pitch() const;
};

#endif // PITCH_DETECTOR_H