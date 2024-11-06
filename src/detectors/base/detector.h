#ifndef DETECTOR_H
#define DETECTOR_H

#include "core/constants.h"

// Template function to check if a value is within a given tolerance
template <typename Type>
bool withinTolerance(Type value, Type tolerance)
{
    return abs(value) < tolerance;
}

// Base behavior for detectors
struct Detector
{
    virtual void reset() = 0;
    virtual void update() = 0;
    virtual ~Detector() = default; // Virtual destructor for proper cleanup
};

#endif // DETECTOR_H