#ifndef SIGNAL_H
#define SIGNAL_H

#include "core/constants.h"

namespace Patterns
{
    typedef bool IsClipping;

    enum class Trigger
    {
        PositiveEdge,
        NegativeEdge,
        None
    };

    enum class PeakDirection
    {
        Rising,
        Falling,
        NoPeak
    };
}

struct Signal
{
private:
    // Owns state
    byte sampleBuffer[2];
    byte delta;

public:
    Signal() : sampleBuffer{ADC_MIN, ADC_MIN} {};
    void update(byte sample);

    // visible state
    byte input() const;
    byte slope() const;

    // pattern analysis
    Patterns::IsClipping isClipping() const;
    Patterns::PeakDirection peakDirection() const;
    Patterns::Trigger isTriggered(byte level = ADC_EQUILIBRIUM) const;
};

#endif // SIGNAL_H
