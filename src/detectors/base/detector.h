#ifndef DETECTOR_H
#define DETECTOR_H

// Template function to check if a value is within a given tolerance
template <typename Type>
bool withinTolerance(Type value, Type tolerance)
{
    return abs(value) < tolerance;
}

// Base behavior for detectors
struct Detector
{
protected:
    virtual void reset() = 0;
    virtual void update() = 0;

public:
    virtual ~Detector() = default; // Virtual destructor for proper cleanup
};

#endif // DETECTOR_H