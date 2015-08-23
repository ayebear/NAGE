// Copyright (C) 2015 Eric Hebert (ayebear)
// This code is licensed under LGPLv3, see LICENSE.txt for details.

#ifndef NAGE_COLORS_H
#define NAGE_COLORS_H

#include <SFML/Graphics/Color.hpp>

namespace ng
{

// Interpolate two colors by a certain amount
sf::Color mixColors(const sf::Color& startColor, const sf::Color& endColor, float ratio = 0.5f);

// Desaturate a color completely (using the average algorithm)
sf::Color desaturate(const sf::Color& color);

// Desaturate a color partially
sf::Color desaturate(const sf::Color& color, float percent);

}

#endif
