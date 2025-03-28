#include "raylib.h"
#include "vector"
#include "map"
#include "string"
#include <iostream>
#include <nlohmann/json.hpp>

struct Tile
{
    int id;
    int tileHeight;
    int tileWidth;
    Texture2D sprite;
};

struct MapLayer 
{
    int id;
    std::vector<Tile> mapLayer;
};

struct Map
{
    int ySize;
    int xSize;

    std::vector<MapLayer> mapMatrix;
};

class MapManager
{
private:
    std::map<const char*, Texture2D> tilesSprite;
    std::map<const char*, Texture2D> tilesSpriteSheet;
    Map map;

public:
    void loadTexture(const char* key, const char* path, bool isSpriteSheet);
    void initMap(const char* pathMap = nullptr);
    void drawMap();

    MapManager();
    ~MapManager();
};