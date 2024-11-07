#ifndef PITCH_DETECTOR_H
#define PITCH_DETECTOR_H

#include "utils.h"
#include "core/signal.h"
#include "detectors/base/detector.h"
#include "detectors/slope/slope.h"
#include "notes.h"

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

// Pattern detector to recognize periodic signal patterns
struct PitchDetector : public Detector
{
private:
    unsigned int clock;
    byte index;
    byte delta[MEASUREMENT_WINDOW];
    unsigned int timer[MEASUREMENT_WINDOW];
    unsigned int period;
    byte lastMatch;
    float fundamentalFreq;
    SignalAnalyzer *signal;
    SlopeDetector *slopeDetector;

public:
    explicit PitchDetector(SignalAnalyzer *analyzer, SlopeDetector *slopeDetector)
        : signal(analyzer),
          slopeDetector(slopeDetector),
          fundamentalFreq(DEFAULT_A4) {}

    enum class Result
    {
        Init,
        Found,
        None
    };

    void reset() override;
    void update() override;
    Result detect(void (*pinWriter)()) const;

    void setFundamentalFreq(float freq);
    float frequency() const;
    Pitch pitch() const;

protected:
    inline Result process();
};

#endif // PITCH_DETECTOR_H