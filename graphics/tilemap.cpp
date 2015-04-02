// Copyright (C) 2014-2015 Eric Hebert (ayebear)
// This code is licensed under LGPLv3, see LICENSE.txt for details.

#include "tilemap.h"
#include "configfile.h"
#include <iostream>

namespace ng
{

TileMap::TileMap()
{
    currentLayer = nullptr;
}

bool TileMap::loadFromConfig(const std::string& filename)
{
    cfg::File config(filename, cfg::File::Warnings | cfg::File::Errors);
    return (config && loadTileset(config("texture"), config("tileWidth").toInt(),
            config("tileHeight").toInt(), config("totalTypes").toInt()));
}

bool TileMap::loadTileset(const std::string& filename, unsigned tileWidth, unsigned tileHeight, unsigned types)
{
    tileSize.x = tileWidth;
    tileSize.y = tileHeight;
    totalTypes = types;
    return texture.loadFromFile(filename);
}

void TileMap::resize(unsigned width, unsigned height)
{
    mapSize.x = width;
    mapSize.y = height;
    totalTiles = mapSize.x * mapSize.y;

    // Resizes all current layers
    for (auto& layer: tiles)
        resize(layer.second);
}

void TileMap::useLayer(int layer)
{
    currentLayer = &tiles[layer];
    resize();
}

void TileMap::set(int layer, unsigned x, unsigned y, unsigned value)
{
    useLayer(layer);
    set(x, y, value);
}

void TileMap::set(unsigned x, unsigned y, unsigned value)
{
    if (currentLayer && inBounds(x, y))
    {
        // Store the value
        currentLayer->tiles[mapSize.x * y + x] = value;

        // Calculate the tile set position to use
        int textureWidth = texture.getSize().x;
        sf::Vector2i tilesetPos(value % (textureWidth / tileSize.x),
                                value / (textureWidth / tileSize.x));

        // Setup the texture coordinates
        sf::Vertex* quad = &currentLayer->vertices[(x + y * mapSize.x) * 4];
        quad[0].texCoords = sf::Vector2f(tilesetPos.x * tileSize.x, tilesetPos.y * tileSize.y);
        quad[1].texCoords = sf::Vector2f((tilesetPos.x + 1) * tileSize.x, tilesetPos.y * tileSize.y);
        quad[2].texCoords = sf::Vector2f((tilesetPos.x + 1) * tileSize.x, (tilesetPos.y + 1) * tileSize.y);
        quad[3].texCoords = sf::Vector2f(tilesetPos.x * tileSize.x, (tilesetPos.y + 1) * tileSize.y);
    }
}

unsigned TileMap::operator()(int layer, unsigned x, unsigned y) const
{
    unsigned value = 0;
    if (inBounds(x, y))
    {
        auto found = tiles.find(layer);
        if (found != tiles.end())
            value = found->second.tiles[mapSize.x * y + x];
    }
    return value;
}

unsigned TileMap::operator()(unsigned x, unsigned y) const
{
    unsigned value = 0;
    if (currentLayer && inBounds(x, y))
        value = currentLayer->tiles[mapSize.x * y + x];
    return value;
}

sf::Vector2u TileMap::getPixelSize() const
{
    return sf::Vector2u(mapSize.x * tileSize.x, mapSize.y * tileSize.y);
}

const sf::Vector2u& TileMap::getMapSize() const
{
    return mapSize;
}

const sf::Vector2u& TileMap::getTileSize() const
{
    return tileSize;
}

sf::FloatRect TileMap::getBoundingBox(unsigned x, unsigned y) const
{
    return sf::FloatRect(x * tileSize.x, y * tileSize.y, tileSize.x, tileSize.y);
}

bool TileMap::inBounds(int x, int y) const
{
    return (x >= 0 && y >= 0 && x < static_cast<int>(mapSize.x) && y < static_cast<int>(mapSize.y));
}

void TileMap::setColor(const sf::Color& color)
{
    vertexColor = color;
    applyColor();
}

unsigned TileMap::getTotalTypes() const
{
    return totalTypes;
}

void TileMap::drawLayer(sf::RenderTarget& target, int layer)
{
    sf::RenderStates states;
    states.transform *= getTransform();
    states.texture = &texture;
    target.draw(tiles[layer].vertices, states);
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &texture;
    for (auto& layer: tiles)
        target.draw(layer.second.vertices, states);
}

void TileMap::resize()
{
    if (currentLayer)
        resize(*currentLayer);
}

void TileMap::resize(TileLayer& layer)
{
    // Setup arrays if they haven't been setup already
    if (layer.tiles.size() != totalTiles)
    {
        layer.tiles.resize(totalTiles);
        layer.vertices.setPrimitiveType(sf::Quads);
        layer.vertices.resize(totalTiles * 4);

        // Setup vertices for this layer
        for (unsigned y = 0; y < mapSize.y; ++y)
        {
            for (unsigned x = 0; x < mapSize.x; ++x)
            {
                sf::Vertex* quad = &layer.vertices[(x + y * mapSize.x) * 4];
                quad[0].position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
                quad[1].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
                quad[2].position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);
            }
        }
    }
}

void TileMap::applyColor()
{
    for (auto& layer: tiles)
    {
        for (unsigned i = 0; i < layer.second.vertices.getVertexCount(); ++i)
            layer.second.vertices[i].color = vertexColor;
    }
}

}
