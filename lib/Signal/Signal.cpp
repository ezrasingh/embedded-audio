#include "Signal.h"

void Signal::update(byte sample)
{
    sampleBuffer[0] = sampleBuffer[1];
    sampleBuffer[1] = sample;
    delta = sampleBuffer[1] - sampleBuffer[0];
};

byte Signal::input() const
{
    return sampleBuffer[1];
};

byte Signal::slope() const
{
    return delta;
}

Patterns::IsClipping Signal::isClipping() const
{
    return (
        sampleBuffer[1] <= ADC_MIN || // clipping below
        sampleBuffer[1] >= ADC_MAX    // clipping above
    );
}

Patterns::PeakDirection Signal::peakDirection() const
{
    if (delta > 0)
        return Patterns::PeakDirection::Rising;
    if (delta < 0)
        return Patterns::PeakDirection::Falling;
    return Patterns::PeakDirection::NoPeak;
}

Patterns::Trigger Signal::isTriggered(byte level) const
{
    if (sampleBuffer[0] < level && sampleBuffer[1] >= level)
        return Patterns::Trigger::PositiveEdge;
    if (sampleBuffer[0] > level && sampleBuffer[1] <= level)
        return Patterns::Trigger::NegativeEdge;
    return Patterns::Trigger::None;
}
