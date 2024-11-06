#include <Arduino.h>
#include "constants.h"

template <typename Type>
bool withinTolerance(Type value, Type tolerance)
{
    return abs(value) < tolerance;
};

struct Detector
{
    virtual void reset() {}
    virtual void update() {}
    virtual ~Detector() {} // Virtual destructor for proper cleanup
};

namespace Patterns
{
    enum Trigger
    {
        PositiveEdge,
        NegativeEdge,
        None,
    };

    enum PeakDirection
    {
        Rising,
        Falling,
        NoPeak,
    };
};

struct SignalAnalyzer
{
private: // owns state
    byte sampleBuffer[2] = {ADC_MIN, ADC_MIN};
    byte delta;

public:
    void update(byte sample)
    {
        sampleBuffer[0] = sampleBuffer[1];
        sampleBuffer[1] = sample;
        delta = sampleBuffer[1] - sampleBuffer[0];
    }
    byte input()
    {
        return sampleBuffer[1];
    }
    byte slope()
    { // slope between two sample moments
        return delta;
    }

    bool isClipping()
    {
        return sampleBuffer[1] == ADC_MIN || sampleBuffer[1] == ADC_MAX;
    }

    Patterns::PeakDirection peakDirection()
    {
        if (delta > 0)
            return Patterns::PeakDirection::Rising;
        if (delta < 0)
            return Patterns::PeakDirection::Falling;
        return Patterns::PeakDirection::NoPeak;
    }

    Patterns::Trigger isTriggered(byte level = ADC_EQUILIBRIUM)
    {
        // if increasing and crossing midpoint
        if (sampleBuffer[0] < level && sampleBuffer[1] >= level)
            return Patterns::Trigger::PositiveEdge;
        // if decreasing and crossing midpoint
        if (sampleBuffer[0] > level && sampleBuffer[1] <= level)
            return Patterns::Trigger::NegativeEdge;

        return Patterns::Trigger::None;
    }
};

struct FrequencyDetector : public Detector
{
private: // owns state
    unsigned int clock;
    unsigned int period;

    SignalAnalyzer *signal;

public:
    FrequencyDetector(SignalAnalyzer *analyzer) : signal(analyzer) {}
    void reset()
    {
        clock = 0;
    }

    void update()
    {
        Patterns::Trigger trigger = signal->isTriggered(ADC_EQUILIBRIUM);

        if (trigger == Patterns::Trigger::PositiveEdge)
        {
            period = clock;
            reset();
        }

        clock++;
    }

    unsigned int frequency()
    {
        // in Hz
        return SAMPLE_RATE / period;
    }
};

struct SlopeDetector : public Detector
{
private: // owns state
    unsigned int time;
    byte maxSlope;

    SignalAnalyzer *signal;

public:
    SlopeDetector(SignalAnalyzer *analyzer) : signal(analyzer) {}
    enum Result
    {
        AboveTolerance,
        WithinTolerance,
        BelowTolerance
    };

    void reset()
    {
        time = 0;     // reset time
        maxSlope = 0; // reset delta
    }

    void update()
    {
        time++; // increment timer
    }

    Result detect()
    { // determine slope condition
        byte slope = signal->slope();
        if (withinTolerance(slope - maxSlope, SLOPE_TOLERANCE))
        {
            return Result::WithinTolerance;
        }
        if (slope > maxSlope)
        {
            maxSlope = slope;
            return Result::AboveTolerance;
        }
        return Result::BelowTolerance;
    }
};

struct LevelDetector : public Detector
{
private:
    unsigned int clock;
    byte maxAmp;
    byte prevMax;

    SignalAnalyzer *signal;

public:
    LevelDetector(SignalAnalyzer *analyzer) : signal(analyzer) {}
    enum Result
    {
        AboveThreshold,
        BelowThreshold
    };
    void reset()
    {
        clock = 0;
        maxAmp = 0;
    }

    void update()
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

    Result detect(byte threshold = AMPLITUDE_THRESHOLD)
    {
        if (prevMax >= threshold)
            return Result::AboveThreshold;
        else
            return Result::BelowThreshold;
    }
};

struct PatternDetector : public Detector
{
private: // owns state
    unsigned int clock;
    byte index;                             // current storage index
    byte delta[MEASUREMENT_WINDOW];         // storage for slope of events
    unsigned int timer[MEASUREMENT_WINDOW]; // storage for timing of events
    unsigned int period;                    // storage for period of wave
    byte lastMatch;                         // counts how many non-matches you've received to reset variables if it's been too long

    SignalAnalyzer *signal;
    SlopeDetector *slopeDetector;

public:
    PatternDetector(
        SignalAnalyzer *analyzer,
        SlopeDetector *slopeDetector) : signal(analyzer), slopeDetector(slopeDetector) {}

    enum Result
    {
        Init,
        Found,
        None
    };

    void reset()
    {
        index = 0;
        clock = 0;
        lastMatch = 0;
        slopeDetector->reset();
    }

    void update()
    {
        clock++;
        slopeDetector->update();
    }

    unsigned int frequency()
    {
        // in Hz
        return SAMPLE_RATE / period;
    }

    Result process()
    { // record new data and reset time
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
            lastMatch = 0; // reset last match counter
            period = 0;    // sum timer values to re-calculate total period
            for (byte i = 0; i < index; i++)
                period += timer[i];
            ;
            return Result::Found;
        }

        index++; // increment index
        if (index >= MEASUREMENT_WINDOW)
            reset();
        return Result::None;
    }

    Result detect(void (*pinWriter)()) // writer abstracts pin behavior
    {
        switch (slopeDetector->detect())
        {
        case SlopeDetector::Result::AboveTolerance:
            lastMatch = 0; // reset last match counter
            clock = 0;     // reset clock
            index = 0;     // reset index
            pinWriter();   // output to pins
            return Result::None;
        case SlopeDetector::Result::WithinTolerance:
            Result res = process();
            if (res == Result::Found)
            {
                // reset new zero index values to compare with
                timer[0] = timer[index];
                delta[0] = delta[index];
                index = 1;   // set index to 1
                pinWriter(); // output to pins
            };
            return res;
        default: // no pattern matched yet
            lastMatch++;
            if (lastMatch >= MEASUREMENT_WINDOW)
                reset();
            return Result::None;
        }
    }
};

struct PitchDetector : public Detector
{
private: // owns state
    SignalAnalyzer *signal;
    LevelDetector *levelDetector;
    PatternDetector *patternDetector;

public:
    PitchDetector(
        SignalAnalyzer *analyzer,
        LevelDetector *levelDetector,
        PatternDetector *patternDetector) : signal(analyzer), levelDetector(levelDetector), patternDetector(patternDetector) {}

    void reset()
    {
        levelDetector->reset();
        patternDetector->reset();
    }

    void update()
    {
        levelDetector->update();
        patternDetector->update();
    }

    void detect(void (*pinWriter)()) // writer abstracts pin behavior
    {
        Patterns::Trigger trigger = signal->isTriggered(ADC_EQUILIBRIUM);

        if (trigger == Patterns::Trigger::PositiveEdge)
        {
            patternDetector->detect(pinWriter);
        }
    }
};
