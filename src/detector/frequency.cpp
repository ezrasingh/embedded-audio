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

unsigned int FrequencyDetector::frequency() const
{
    return (period > 0) ? SAMPLE_RATE / period : 0;
}