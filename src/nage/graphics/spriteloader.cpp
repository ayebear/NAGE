// Copyright (C) 2014-2015 Eric Hebert (ayebear)
// This code is licensed under LGPLv3, see LICENSE.txt for details.

#include "nage/graphics/spriteloader.h"
#include <configfile.h>
#include <iostream>

namespace ng
{

std::map<std::string, sf::Texture> SpriteLoader::textures;

SpriteLoader::SpriteLoader()
{
}

SpriteLoader::SpriteLoader(const std::string& configFilename)
{
    loadFromConfig(configFilename);
}

bool SpriteLoader::load(const std::string& name, const std::string& textureFilename, bool resetRect)
{
    bool status;
    auto& texture = loadTexture(textureFilename, status);
    if (status)
        sprites[name].setTexture(texture, resetRect);
    return status;
}

bool SpriteLoader::load(sf::Sprite& sprite, const std::string& textureFilename, bool resetRect)
{
    bool status;
    auto& texture = loadTexture(textureFilename, status);
    if (status)
        sprite.setTexture(texture, resetRect);
    return status;
}

bool SpriteLoader::loadFromConfig(const std::string& configFilename)
{
    bool status = true;
    cfg::File config(configFilename);
    for (auto& option: config.getSection())
        status = load(option.first, option.second.toString());
    return status;
}

sf::Sprite& SpriteLoader::getSprite(const std::string& name)
{
    return sprites[name];
}

sf::Texture& SpriteLoader::getTexture(const std::string& filename)
{
    return textures[filename];
}

sf::Sprite& SpriteLoader::operator()(const std::string& name)
{
    return sprites[name];
}

bool SpriteLoader::preloadTexture(const std::string& filename)
{
    bool status;
    loadTexture(filename, status);
    return status;
}

sf::Texture& SpriteLoader::loadTexture(const std::string& filename, bool& status)
{
    status = true;
    // Only load the texture if it hasn't been loaded yet
    bool notLoaded = (textures.find(filename) == textures.end());
    auto& texture = textures[filename];
    if (notLoaded)
    {
        std::cout << "Loading new texture: " << filename << "...";
        status = texture.loadFromFile(filename);
        if (status)
            std::cout << " Done.\n";
        else
            std::cout << " Error!\n";
    }
    return texture;
}

}
