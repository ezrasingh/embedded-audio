#include "pitch.h"

float Pitch::cents() const
{
    return deviation;
}

String Pitch::note() const
{
    return NOTES(index);
}

void PitchDetector::reset()
{
    index = 0;
    clock = 0;
    lastMatch = 0;
    slopeDetector->reset();
}

void PitchDetector::update()
{
    clock++;
    slopeDetector->update();
}

float PitchDetector::frequency() const
{
    return (period > 0) ? static_cast<float>(ADC_SAMPLE_RATE) / static_cast<float>(period) : 0;
}

void PitchDetector::setFundamentalFreq(float freq)
{
    fundamentalFreq = freq;
}

// Function to determine the closest note to a given frequency
Pitch PitchDetector::pitch() const
{
    float freq = frequency(); // Get frequency from pattern detector
    // Calculate the number of half steps from A4
    float halfStepsFromA4 = round(12.0f * log2(freq / fundamentalFreq));
    // Calculate the frequency of the closest note
    float closestFreq = fundamentalFreq * pow(2.0f, halfStepsFromA4 / 12.0f);
    // Calculate deviation in cents
    float centsDeviation = 1200.0f * log2(freq / closestFreq);
    // Map halfStepsFromA4 to a note within the 12-tone system
    int noteIndex = static_cast<int>(halfStepsFromA4) % 12;
    if (noteIndex < 0)
        noteIndex += 12; // Handle negative wraparound for notes below A4
    return Pitch(noteIndex, centsDeviation);
}

PitchDetector::Result PitchDetector::detect(void (*pinWriter)())
{
    switch (slopeDetector->detect())
    {
    case SlopeDetector::Result::AboveTolerance:
    {
        lastMatch = 0;
        clock = 0;
        index = 0;
        pinWriter();
        return Result::None;
    }

    case SlopeDetector::Result::WithinTolerance:
    {
        Result res = process();
        if (res == Result::Found)
        {
            timer[0] = timer[index];
            delta[0] = delta[index];
            index = 1;
            pinWriter();
        }
        return res;
    }

    case SlopeDetector::Result::BelowTolerance:
    default:
        lastMatch++;
        if (lastMatch >= MEASUREMENT_WINDOW)
        {
            reset();
        }
        return Result::None;
    }
}

inline PitchDetector::Result PitchDetector::process()
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
