// Copyright (C) 2014-2015 Eric Hebert (ayebear)
// This code is licensed under LGPLv3, see LICENSE.txt for details.

#include "nage/gui/buttonmap.h"

namespace ng
{

ButtonMap::ButtonMap()
{
}

Button& ButtonMap::operator[](const std::string& name)
{
    return buttons[name];
}

void ButtonMap::handleMouseEvent(const sf::Event& event, const sf::Vector2f& pos)
{
    for (auto& button: buttons)
        button.second.handleMouseEvent(event, pos);
}

void ButtonMap::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    for (auto& button: buttons)
        window.draw(button.second);
}

}
