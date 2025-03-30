#include "raylib.h"
#include <vector>
#include <map>
#include <string>
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
	int sizeX = 1;
	int sizeY = 1;

	int tileWidth;
    int tileHeight;
	Texture2D texture;

	void drawSprite(const Tile& tile, float posX, float posY);
};

class MapManager
{
private:
	Vector2 offset;
	int screenWidth;
	int gridCellWidth;
	int gridCellHeight;
	SpriteSheet spriteSheet;
	
	void serializeMap();
	Map deserializeMap(const char *pathMap);
	void drawGrid(Vector2 mousePos, int row, int col, float x, float y);
	void loadTexture(const char* key, const char* path, int tileWidth, int tileHeight);
	
public:
	Map map;
	
	void init(const char *pathMap, int screenWidth);
	void draw();

	MapManager();
	~MapManager();
};