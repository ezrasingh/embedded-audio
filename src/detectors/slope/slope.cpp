#include "slope.h"

void SlopeDetector::reset()
{
    clock = 0;
    maxSlope = 0;
}

void SlopeDetector::update()
{
    clock++;
}

SlopeDetector::Result SlopeDetector::detect()
{
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