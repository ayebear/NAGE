// Copyright (C) 2015 Eric Hebert (ayebear)
// This code is licensed under LGPLv3, see LICENSE.txt for details.

#include "nage/graphics/colors.h"
#include "nage/misc/utils.h"
// #include <cmath>

namespace ng
{

sf::Color mixColors(const sf::Color& startColor, const sf::Color& endColor, float ratio)
{
    return sf::Color(interpolate(startColor.r, endColor.r, ratio),
                     interpolate(startColor.g, endColor.g, ratio),
                     interpolate(startColor.b, endColor.b, ratio),
                     interpolate(startColor.a, endColor.a, ratio));
}

sf::Color desaturate(const sf::Color& color)
{
    sf::Uint8 avg = (static_cast<int>(color.r) +
                     static_cast<int>(color.g) +
                     static_cast<int>(color.b)) / 3;
    return {avg, avg, avg, color.a};
}

sf::Color desaturate(const sf::Color& color, float ratio)
{
    return mixColors(color, desaturate(color), ratio);
}

}
