#include "pitch.h"

const String NOTES[12] = {"A", "A#/Bb", "B", "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab"};

float Pitch::cents() const
{
    return deviation;
}

String Pitch::note() const
{
    return NOTES[index];
}

void PitchDetector::reset()
{
    levelDetector->reset();
    patternDetector->reset();
}

void PitchDetector::update()
{
    levelDetector->update();
    patternDetector->update();
}

void PitchDetector::detect(void (*pinWriter)())
{
    Patterns::Trigger trigger = signal->isTriggered(ADC_EQUILIBRIUM);

    if (trigger == Patterns::Trigger::PositiveEdge)
    {
        patternDetector->detect(pinWriter);
    }

    update();
}

void PitchDetector::setFundamentalFreq(float freq)
{
    fundamentalFreq = freq;
}

// Function to determine the closest note to a given frequency
Pitch PitchDetector::pitch() const
{
    float freq = patternDetector->frequency(); // Get frequency from pattern detector
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