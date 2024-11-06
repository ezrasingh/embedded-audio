#include "pattern.h"

void PatternDetector::reset()
{
    index = 0;
    clock = 0;
    lastMatch = 0;
    slopeDetector->reset();
}

void PatternDetector::update()
{
    clock++;
    slopeDetector->update();
}

float PatternDetector::frequency() const
{
    return (period > 0) ? static_cast<float>(SAMPLE_RATE) / static_cast<float>(period) : 0;
}

PatternDetector::Result PatternDetector::process()
{
    byte nextDelta = signal->slope();
    delta[index] = nextDelta;
    timer[index] = clock;
    clock = 0;

    if (index == 0)
    {
        lastMatch = 0;
        index++;
        return Result::Init;
    }

    if (withinTolerance(timer[0] - timer[index], static_cast<unsigned int>(TIMER_TOLERANCE)) &&
        withinTolerance(delta[0] - nextDelta, SLOPE_TOLERANCE))
    {
        lastMatch = 0;
        period = 0;
        for (byte i = 0; i < index; i++)
        {
            period += timer[i];
        }
        return Result::Found;
    }

    index++;
    if (index >= MEASUREMENT_WINDOW)
    {
        reset();
    }
    return Result::None;
}

PatternDetector::Result PatternDetector::detect(void (*pinWriter)())
{
    switch (slopeDetector->detect())
    {
    case SlopeDetector::Result::AboveTolerance:
        lastMatch = 0;
        clock = 0;
        index = 0;
        pinWriter();
        return Result::None;
    case SlopeDetector::Result::WithinTolerance:
        Result res = process();
        if (res == Result::Found)
        {
            timer[0] = timer[index];
            delta[0] = delta[index];
            index = 1;
            pinWriter();
        }
        return res;
    default:
        lastMatch++;
        if (lastMatch >= MEASUREMENT_WINDOW)
        {
            reset();
        }
        return Result::None;
    }
}