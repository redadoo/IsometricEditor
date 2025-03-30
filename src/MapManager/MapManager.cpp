#include "MapManager.hpp"

MapManager::MapManager() {}

MapManager::~MapManager() {}

void MapManager::init(const char *pathMap, int screenWidth)
{
	this->screenWidth = screenWidth;
	this->offset = { static_cast<float>(this->screenWidth / 2), 100 };

	if (pathMap != nullptr)
		this->map = deserializeMap(pathMap);
	else 
		this->map = Map();

	this->gridCellWidth = this->map.tileWidth;
	this->gridCellHeight = this->map.tileHeight / 2;
}

void MapManager::serializeMap()
{
	std::ofstream outFile("map_out.json");
	if (!outFile.is_open()) 
		throw std::runtime_error("Cannot open output file for writing");

	outFile << "{\n";
	outFile << "    \"map\": {\n";
	outFile << "        \"version\": \"1.10\",\n";
	outFile << "        \"width\": " << map.width << ",\n";
	outFile << "        \"height\": " << map.height << ",\n";
	outFile << "        \"tilewidth\": " << map.tileWidth << ",\n";
	outFile << "        \"tileheight\": " << map.tileHeight << ",\n";
	outFile << "        \"tilesets\": [\n";
	outFile << "            {\n";
	outFile << "                \"source\": \"assets/tilesets/spritesheet.png\"\n";
	outFile << "            }\n";
	outFile << "        ],\n";
	outFile << "        \"layers\": [\n";

	for (size_t layerIndex = 0; layerIndex < map.layers.size(); ++layerIndex) 
	{
		const auto& layer = map.layers[layerIndex];
		outFile << "            {\n";
		outFile << "                \"id\": " << layer.id << ",\n";
		outFile << "                \"data\": \n\t\t\t\t[\n";

		for (size_t rowIndex = 0; rowIndex < layer.tiles.size(); ++rowIndex) 
		{
			outFile << "\t\t\t\t\t";

			const auto& row = layer.tiles[rowIndex];
			if (rowIndex > 0) 
				outFile << "";

			outFile << "[";
			
			for (size_t colIndex = 0; colIndex < row.size(); ++colIndex) 
			{
				const auto& tile = row[colIndex];
				outFile << tile.id;
				if (colIndex < row.size() - 1) 
					outFile << ", ";
			}

			outFile << "]";
			if (rowIndex < layer.tiles.size() - 1) 
				outFile << ",";
			outFile << "\n";
		}

		outFile << "                ]\n";
		outFile << "            }";
		if (layerIndex < map.layers.size() - 1)
			outFile << ",";
		outFile << "\n";
	}
	outFile << "        ]\n";
	outFile << "    }\n";
	outFile << "}\n";
	outFile.close();
}

Map MapManager::deserializeMap(const char *pathMap)
{
	std::ifstream file(pathMap);
	if (!file.is_open()) 
		throw std::runtime_error("Cannot open map file");

	nlohmann::json j;
	file >> j;

	auto m = j["map"];

	Map newMap;
	newMap.width      = m["width"].get<int>();
	newMap.height     = m["height"].get<int>();
	newMap.tileWidth  = m["tilewidth"].get<int>();
	newMap.tileHeight = m["tileheight"].get<int>();

	std::string tileSheetSource = m["tilesets"][0]["source"].get<std::string>();
	loadTexture("atlas", tileSheetSource.c_str(), newMap.tileWidth, newMap.tileHeight);

	for (auto& layerJson : m["layers"]) 
	{
		MapLayer layer;
		layer.id = layerJson["id"].get<int>();
		
		const auto& data = layerJson["data"];
		size_t rowCount = data.size();

		if (rowCount > 0)
		{
			size_t colCount = data[0].size();
			
			for (size_t row = 0; row < rowCount; ++row)
			{
				std::vector<Tile> rowTiles;
				
				for (size_t col = 0; col < colCount; ++col)
				{
					Tile tile;
					tile.id = data[row][col].get<int>();
					rowTiles.push_back(tile);
				}
				
				layer.tiles.push_back(rowTiles);
			}
		}
		
		newMap.layers.push_back(layer);
	}

	return newMap;
}

void MapManager::loadTexture(const char *key, const char *path, int tileWidth, int tileHeight)
{
	Image spriteImage = LoadImage(path);
	if (spriteImage.data != NULL)
	{
		Texture2D sprite = LoadTextureFromImage(spriteImage);
		UnloadImage(spriteImage); 
		spriteSheet.texture = sprite;
		spriteSheet.sizeX = spriteImage.width / tileWidth;
		spriteSheet.sizeY = spriteImage.height / tileHeight;
	}
	else
		std::cout << "error\n" ;
}

void MapManager::draw()
{
	Vector2 mousePos = GetMousePosition();
	int offSet = map.tileHeight / 2; 

	for (size_t layerIndex = 0; layerIndex < map.layers.size(); ++layerIndex)
	{
		for (int row = 0; row < this->map.height; row++)
		{
			for (int col = 0; col < this->map.width; col++)
			{
				
				float x = (col - row) * (gridCellWidth / 2.0f) + offset.x;
				float y = (col + row) * (gridCellHeight / 2.0f) + offset.y;
				
				if (layerIndex == 0)
					this->drawGrid(mousePos, row, col, x, y);

				this->spriteSheet.drawSprite(
					map.layers[layerIndex].tiles[row][col],
					x - offSet, 
					y - offSet
				);
			}
		}
	}
}

void MapManager::drawGrid(Vector2 mousePos, int row, int col, float x, float y)
{
	Vector2 top    = { x, y };
	Vector2 right  = { x + gridCellWidth / 2.0f, y + gridCellHeight / 2.0f };
	Vector2 bottom = { x, y + gridCellHeight };
	Vector2 left   = { x - gridCellWidth / 2.0f, y + gridCellHeight / 2.0f };

	DrawLineV(top, right, BLACK);
	DrawLineV(right, bottom, BLACK);
	DrawLineV(bottom, left, BLACK);
	DrawLineV(left, top, BLACK);

	if (false)
	{
		DrawTriangle(right, top, bottom, RED);
		DrawTriangle(left, bottom, top, RED);
	}
}

void SpriteSheet::drawSprite(const Tile& tile, float posX, float posY)
{
	int tileIndex = tile.id - 1; 
	int tileSpritecol = tileIndex % sizeX;
	int tileSpriterow = tileIndex / sizeY;
	float y = tileSpriterow * 32;
	float x = tileSpritecol * 32;
	Rectangle rec = { x, y, 32, 32 };
	DrawTextureRec(this->texture, rec, {posX, posY}, WHITE);
}

