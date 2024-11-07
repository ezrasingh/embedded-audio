#ifndef PITCH_DETECTOR_H
#define PITCH_DETECTOR_H

#include <Signal.h>
#include "core/utils.h"
#include "../base/detector.h"
#include "../slope/slope.h"
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
    float refFreq;
    Signal *signal;
    SlopeDetector *slopeDetector;

public:
    explicit PitchDetector(Signal *signal, SlopeDetector *slopeDetector)
        : refFreq(DEFAULT_A4),
          signal(signal),
          slopeDetector(slopeDetector)
    {
    }

    enum class Result
    {
        Init,
        Found,
        None
    };

    void reset() override;
    void update() override;
    Result detect(void (*pinWriter)());

    void setRefFreq(float freq);
    float frequency() const;
    Pitch pitch() const;

protected:
    inline Result process();
};

#endif // PITCH_DETECTOR_H
