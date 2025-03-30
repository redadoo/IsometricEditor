#include "raylib.h"
#include "vector"
#include "map"
#include "string"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cmath>
struct Tile 
{
    int id;
    int textureId;
	Vector2 pos;
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
	std::map<int, SpriteSheet> tilesSpriteSheet;
	const int gridCellWidth = 32;
	const int gridCellHeight = 16;
	int screenWidth;
	Vector2 offset;
	
	void serializeMap();
	Map deserializeMap(const char *pathMap);
	void drawGrid(Vector2 mousePos, int row, int col, float x, float y);
	void loadTexture(const char* key, const char* path, bool isSpriteSheet);
	nlohmann::json generateTilesetJson(const char* spritesheetPath, int tileWidth, int tileHeight);
	void initMap(const char* pathMap = nullptr);
	
public:
	Map map;
	void init(const char *pathMap, int screenWidth);
	void draw();

	MapManager();
	~MapManager();
};