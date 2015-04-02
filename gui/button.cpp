// Copyright (C) 2014-2015 Eric Hebert (ayebear)
// This code is licensed under LGPLv3, see LICENSE.txt for details.

#include "button.h"
#include "nage/graphics/colorcode.h"
#include <iostream>

namespace ng
{

Button::Button()
{
    hovered = false;
    pressed = false;
    beingPressed = false;
    mode = Mode::Normal;
    linkedButtons = nullptr;
    setFontSize();
    setOutlineThickness();
    setColor("outline", "#FFFC");
    setColor("fill", "#444C");
    setColor("text", "#FFF");
    setColor("outlinePressed", "#000C");
    setColor("fillHovered", "#888C");
    updateColors();
}

void Button::setup(const sf::Font& f, const sf::Vector2f& pos, const sf::Vector2u& size, const std::string& text)
{
    setFont(f);
    setPosition(pos);
    setSize(size);
    setText(text);
}

void Button::handleMouseEvent(const sf::Event& event, const sf::Vector2f& pos)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (collisionBox.contains(pos))
        {
            beingPressed = true;
            updateColors();
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        bool wasPressed = (beingPressed && collisionBox.contains(pos));
        beingPressed = false;
        if (wasPressed)
        {
            if (mode == Mode::Toggle)
                pressed = !pressed;
            else if (mode == Mode::Sticky)
            {
                pressed = true;
                unpressLinkedButtons();
            }
            if (pressedCallback)
                pressedCallback(*this);
        }
        updateColors();
    }
    else if (event.type == sf::Event::MouseMoved)
    {
        hovered = collisionBox.contains(pos);
        updateColors();
    }
}

void Button::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(box);
    window.draw(text);
}

void Button::setColor(const std::string& setting, const std::string& color)
{
    setColor(setting, ColorCode(color).toColor());
}

void Button::setColor(const std::string& setting, const sf::Color& color)
{
    if (setting == "outline")
        outlineColors[0] = color;
    else if (setting == "fill")
        fillColors[0] = color;
    else if (setting == "text")
        text.setColor(color);
    else if (setting == "outlinePressed")
        outlineColors[1] = color;
    else if (setting == "fillHovered")
        fillColors[1] = color;
}

void Button::setFont(const sf::Font& f)
{
    text.setFont(f);
}

void Button::setFontSize(unsigned size)
{
    text.setCharacterSize(size);
}

void Button::setOutlineThickness(float thickness)
{
    box.setOutlineThickness(thickness);
}

void Button::setPosition(const sf::Vector2f& pos)
{
    box.setPosition(pos);
    collisionBox.left = pos.x;
    collisionBox.top = pos.y;
    updateTextPosition();
}

void Button::setSize(const sf::Vector2u& size)
{
    box.setSize(sf::Vector2f(size.x, size.y));
    collisionBox.width = size.x;
    collisionBox.height = size.y;
    updateTextPosition();
}

void Button::setMode(Mode m)
{
    mode = m;
}

void Button::linkButtons(std::vector<Button>& buttons)
{
    linkedButtons = &buttons;
}

void Button::setText(const std::string& str)
{
    text.setString(str);
    updateTextPosition();
}

std::string Button::getText() const
{
    return text.getString().toAnsiString();
}

void Button::setPressed(bool state)
{
    pressed = state;
    updateColors();
}

bool Button::isPressed() const
{
    return pressed;
}

void Button::setPressedCallback(CallbackType callback)
{
    pressedCallback = callback;
}

void Button::updateColors()
{
    box.setFillColor(fillColors[hovered]);
    box.setOutlineColor(outlineColors[pressed || beingPressed]);
}

void Button::updateTextPosition()
{
    auto boxPos = box.getPosition();
    auto boxSize = box.getSize();
    text.setPosition(boxPos);
    auto textBounds = text.getGlobalBounds();
    sf::Vector2f offset((boxSize.x - textBounds.width) / 2.0f - 8,
                        (boxSize.y - textBounds.height) / 2.0f - 8);
    text.setPosition(boxPos.x + offset.x, boxPos.y + offset.y);

    /*if (text.getString().toAnsiString() == "Objects")
    {
        std::cout << "offset = (" << offset.x << ", " << offset.y << "), textSize = (" << textBounds.width << ", " << textBounds.height << ")\n";
        std::cout << "boxSize = (" << boxSize.x << ", " << boxSize.y << "), boxPos.x = " << boxPos.x << "\n";
        // offset = (45, 12), textSize = (60, 16)
        // boxSize = (150, 40), boxPos.x = 166
    }*/
}

void Button::unpressLinkedButtons()
{
    if (linkedButtons)
    {
        for (auto& b: *linkedButtons)
        {
            if (&b != this)
                b.setPressed(false);
        }
    }
}

}
