#include "signal.h"

void SignalAnalyzer::update(byte sample)
{
    sampleBuffer[0] = sampleBuffer[1];
    sampleBuffer[1] = sample;
    delta = sampleBuffer[1] - sampleBuffer[0];
};

byte SignalAnalyzer::input() const
{
    return sampleBuffer[1];
};

byte SignalAnalyzer::slope() const
{
    return delta;
}

Patterns::IsClipping SignalAnalyzer::isClipping() const
{
    return (
        sampleBuffer[1] <= ADC_MIN || // clipping below
        sampleBuffer[1] >= ADC_MAX    // clipping above
    );
}

Patterns::PeakDirection SignalAnalyzer::peakDirection() const
{
    if (delta > 0)
        return Patterns::PeakDirection::Rising;
    if (delta < 0)
        return Patterns::PeakDirection::Falling;
    return Patterns::PeakDirection::NoPeak;
}

Patterns::Trigger SignalAnalyzer::isTriggered(byte level) const
{
    if (sampleBuffer[0] < level && sampleBuffer[1] >= level)
        return Patterns::Trigger::PositiveEdge;
    if (sampleBuffer[0] > level && sampleBuffer[1] <= level)
        return Patterns::Trigger::NegativeEdge;
    return Patterns::Trigger::None;
}
