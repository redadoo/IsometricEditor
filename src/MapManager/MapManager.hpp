#include "SpriteSheet.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cmath>

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
	Tile* getTile(int row, int col, int layerId);
};

class MapManager
{
private:
	int gridCellWidth;
	int gridCellHeight;
	SpriteSheet spriteSheet;

	void serializeMap();
    void setEmptyMap(const char *spriteSheetPath);
    void deserializeMap(const char *pathMap);
    void drawGrid(Vector2 mousePos, int row, int col, float x, float y);
	
public:
	Map map;
	
	void init(const char *pathMap, int screenWidth);
	void draw(Vector2 mousePos);

	MapManager();
	~MapManager();
    void LoadMap(const char *pathMap, int screenWidth);
    void newMap(const char *pathMap, int screenWidth);
};