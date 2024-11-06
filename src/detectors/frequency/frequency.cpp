#include "frequency.h"

void FrequencyDetector::reset()
{
    clock = 0;
}

void FrequencyDetector::update()
{
    Patterns::Trigger trigger = signal->isTriggered(ADC_EQUILIBRIUM);

    if (trigger == Patterns::Trigger::PositiveEdge)
    {
        period = clock;
        reset();
    }

    clock++;
}

float FrequencyDetector::frequency() const
{
    return (period > 0) ? static_cast<float>(SAMPLE_RATE) / static_cast<float>(period) : 0;
}