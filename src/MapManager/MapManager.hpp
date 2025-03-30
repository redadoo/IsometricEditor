#include "raylib.h"
#include "vector"
#include "map"
#include "string"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

struct Tile 
{
    int id;
    int textureId; 
};

struct MapLayer 
{
    int id;
    std::vector<std::vector<Tile>> tiles;
};

struct Map 
{
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    std::vector<MapLayer> layers;
};

struct SpriteSheet
{
	int tileWidth;
    int tileHeight;
	Texture2D texture;

	void drawSprite(Rectangle rec, Vector2 pos);
	void drawSprite(const Tile& tile, int isoX, int isoY);
};

class MapManager
{
private:
	std::map<int, Texture2D> tilesSprite;
	std::map<int, SpriteSheet> tilesSpriteSheet;
	
	void serializeMap();
	Map deSerializeMap(const char *pathMap);
	
public:
	Map map;
	void loadTexture(const char* key, const char* path, bool isSpriteSheet);
	nlohmann::json generateTilesetJson(const char* spritesheetPath, int tileWidth, int tileHeight);
	void initMap(const char* pathMap = nullptr);
	void drawMap();
	MapManager();
	~MapManager();
};