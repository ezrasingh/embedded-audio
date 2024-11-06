#ifndef SIGNAL_H
#define SIGNAL_H

#include "constants.h"

namespace Patterns
{
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

struct SignalAnalyzer
{
private:
    // Owns state
    byte sampleBuffer[2];
    byte delta;

public:
    SignalAnalyzer();

    void update(byte sample);
    byte input() const;
    byte slope() const;
    bool isClipping() const;
    Patterns::PeakDirection peakDirection() const;
    Patterns::Trigger isTriggered(byte level = ADC_EQUILIBRIUM) const;
};

#endif // SIGNAL_H
