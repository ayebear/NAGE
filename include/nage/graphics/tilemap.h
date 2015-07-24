// Copyright (C) 2014-2015 Eric Hebert (ayebear)
// This code is licensed under LGPLv3, see LICENSE.txt for details.

#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

namespace ng
{

/*
This class handles storing and drawing a visual map of tiles.
Tiles are represented with an ID
The tile size and map size can be set.
    Note: Changing this after setting the tiles will reset everything.
A tileset (in a single texture) must be specified with the tile size, as well as the
    total number of tiles. The rest is automatically calculated (tiles per row, etc.)
It uses vertex arrays for performance.
    Note: This means that only one texture can be used.
Multiple layers are supported, and you can use your own integer IDs for each layer.
Animation support could be added in the future if needed.
*/
class TileMap: public sf::Drawable, public sf::Transformable
{
    public:
        TileMap();

        bool loadFromConfig(const std::string& filename);

        // Loads a texture to use for the tile set
        bool loadTileset(const std::string& filename, unsigned tileWidth, unsigned tileHeight, unsigned types = 0, unsigned padding = 0);

        // Resizes all of the layers to the same size
        void resize(unsigned width, unsigned height);

        // Sets the "current" layer (so the layer doesn't have to always be specified)
        void useLayer(int layer);

        // Sets a tile to a certain "visual" ID
        void set(int layer, unsigned x, unsigned y, unsigned value);
        void set(unsigned x, unsigned y, unsigned value);

        // Returns the visual ID of a tile
        unsigned operator()(int layer, unsigned x, unsigned y) const;
        unsigned operator()(unsigned x, unsigned y) const;

        // The size of the tile map in pixels (3840x2160 for example)
        sf::Vector2u getPixelSize() const;

        // The size of the tile map in tiles (32x24 for example)
        const sf::Vector2u& getMapSize() const;

        // The size of a single tile in pixels (128x128 for example)
        const sf::Vector2u& getTileSize() const;

        // Returns the bounding box rectangle of a tile
        sf::FloatRect getBoundingBox(unsigned x, unsigned y) const;

        // Returns the graphical center position of a tile from coordinates
        template <typename T>
        sf::Vector2<T> getCenterPoint(unsigned x, unsigned y) const;

        // Returns the graphical center position of a tile from a vector
        template <typename T1, typename T2>
        sf::Vector2<T1> getCenterPoint(const sf::Vector2<T2>& pos) const;

        // Returns true if the coordinates are in bounds of the tile map
        bool inBounds(int x, int y) const;

        // Applies a color to all vertices
        void setColor(const sf::Color& color);

        // Returns total number of unique visual IDs
        unsigned getTotalTypes() const;

        // Draws a single layer of the tile map
        void drawLayer(sf::RenderTarget& target, int layer);

        // Draws all of the layers of the tile map in order
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        struct TileLayer
        {
            sf::VertexArray vertices; // Graphical tiles
            std::vector<unsigned> tiles; // Tile IDs
        };

        void resize();
        void resize(TileLayer& layer);
        void applyColor();

        unsigned totalTiles; // Total # of tiles in 1 layer
        unsigned totalTypes; // Unique visual IDs
        unsigned tilePadding; // Amount of padding in pixels
        sf::Vector2u mapSize; // In # of tiles
        sf::Vector2u tileSize; // In pixels
        sf::Texture texture; // The tile set
        std::map<int, TileLayer> tiles;
        TileLayer* currentLayer; // Points to the last layer used
        sf::Color vertexColor; // Color applied to all vertices
};

template <typename T>
sf::Vector2<T> TileMap::getCenterPoint(unsigned x, unsigned y) const
{
    return sf::Vector2<T>(x * tileSize.x + (tileSize.x / static_cast<T>(2)),
                          y * tileSize.y + (tileSize.y / static_cast<T>(2)));
}

template <typename T1, typename T2>
sf::Vector2<T1> TileMap::getCenterPoint(const sf::Vector2<T2>& pos) const
{
    return getCenterPoint<T1>(static_cast<T1>(pos.x), static_cast<T1>(pos.y));
}

}

#endif
