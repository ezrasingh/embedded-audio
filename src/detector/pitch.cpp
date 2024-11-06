#include "pitch.h"

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
}