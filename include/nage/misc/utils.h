// Copyright (C) 2014-2015 Eric Hebert (ayebear)
// This code is licensed under LGPLv3, see LICENSE.txt for details.

#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <string>

// Creates an std::function of most methods of type void(void)
#define ngBind(method) [&](){ method(); }

// Creates for loops with ranges
#define forRange(var, start, end) for (long var = (start); var < (end); ++var)

namespace ng
{

static const std::string emptyStr{""};

// Returns an alternate value when dividing by 0
template <class T>
T safeDivide(T numerator, T denominator, T errorValue)
{
    if (denominator)
        return (numerator / denominator);
    return errorValue;
}

// Ensures that a value is less than or equal to its upper limit
template <typename A, typename B>
bool clampLT(A& val, B limit)
{
    if (val < static_cast<A>(limit))
    {
        val = static_cast<A>(limit);
        return true;
    }
    return false;
}

// Ensures that a value is greater than or equal to its lower limit
template <typename A, typename B>
bool clampGT(A& val, B limit)
{
    if (val > static_cast<A>(limit))
    {
        val = static_cast<A>(limit);
        return true;
    }
    return false;
}

// Ensures that a value is in between a min and a max
template <typename T>
bool clamp(T& val, T min, T max)
{
    return (clampLT(val, min) || clampGT(val, max));
}

// Returns true if a float/double is approximately equal within a certain tolerance range
// Tolerance is +/-, so a tolerance of 0.01 will be a total tolerance range of 0.02
template <typename T>
bool approxEqual(T val1, T val2, T tolerance = 0.01)
{
    return (val1 + tolerance >= val2 && val1 - tolerance <= val2);
}

template <typename T>
T interpolate(T start, T end, float ratio)
{
    clamp<float>(ratio, 0.0f, 1.0f);
    T change = (start - end) * ratio;
    return (start - change);
}

}

#endif
