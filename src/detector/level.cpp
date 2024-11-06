#include "level.h"

void LevelDetector::reset()
{
    clock = 0;
    maxAmp = 0;
}

void LevelDetector::update()
{
    byte input = signal->input();
    byte amplitude = abs(ADC_EQUILIBRIUM - input);
    if (amplitude > maxAmp)
    {
        maxAmp = amplitude;
    }

    clock++;

    if (clock >= 1000)
    {
        prevMax = maxAmp;
        reset();
    }
}

LevelDetector::Result LevelDetector::detect(byte threshold) const
{
    return (prevMax >= threshold) ? Result::AboveThreshold : Result::BelowThreshold;
}