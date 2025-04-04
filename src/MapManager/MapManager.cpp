#include "MapManager.hpp"

MapManager::MapManager() {}

MapManager::~MapManager() {}

void MapManager::LoadMap(const char *pathMap, int screenWidth)
{
	deserializeMap(pathMap);
	this->gridCellWidth = this->map.tileWidth;
	this->gridCellHeight = this->map.tileHeight / 2;
}

void MapManager::newMap(const char *spriteSheetPath, int screenWidth)
{
	setEmptyMap(spriteSheetPath);
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

void MapManager::setEmptyMap(const char *spriteSheetPath)
{
	this->map.width = 30;
	this->map.height = 30;
	this->map.tileWidth = 32;
	this->map.tileHeight = 32;
	this->spriteSheet.init(spriteSheetPath, this->map.tileWidth, this->map.tileHeight);

	MapLayer layer;
	layer.id = 0;
	for (size_t row = 0; row < this->map.width; ++row)
	{
		std::vector<Tile> rowTiles;

		for (size_t col = 0; col < this->map.height; ++col)
		{
			Tile tile;
			tile.id = 0;
			rowTiles.push_back(tile);
		}
		
		layer.tiles.push_back(rowTiles);
	}
	this->map.layers.push_back(layer);
}

void MapManager::deserializeMap(const char *pathMap)
{
	std::ifstream file(pathMap);
	if (!file.is_open()) 
		throw std::runtime_error("Cannot open map file");

	nlohmann::json j;
	file >> j;

	auto m = j["map"];

	this->map.width = m["width"].get<int>();
	this->map.height = m["height"].get<int>();
	this->map.tileWidth = m["tilewidth"].get<int>();
	this->map.tileHeight = m["tileheight"].get<int>();

	std::string tileSheetSource = m["tilesets"][0]["source"].get<std::string>();
	this->spriteSheet.init(tileSheetSource.c_str(), this->map.tileWidth, this->map.tileHeight);

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
		this->map.layers.push_back(layer);
	}
}

void MapManager::draw(Vector2 mousePos)
{
	for (size_t layerIndex = 0; layerIndex < map.layers.size(); ++layerIndex)
	{
		for (int row = 0; row < this->map.height; row++)
		{
			for (int col = 0; col < this->map.width; col++)
			{
				float x = (col - row) * (gridCellWidth / 2.0f);
				float y = (col + row) * (gridCellHeight / 2.0f);
				
				if (layerIndex == 0)
					this->drawGrid(mousePos, row, col, x + gridCellWidth / 2, y + gridCellHeight / 2 );
				
				const Tile* tile = this->map.getTile(row,col,layerIndex);
				this->spriteSheet.drawSprite(tile->id, x, y);
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

Tile* Map::getTile(int row, int col, int layerId)
{
    if (layerId < 0 || layerId >= static_cast<int>(layers.size()))
    {
        std::cerr << "Error: Layer ID " << layerId << " is out of bounds." << std::endl;
        return nullptr;
    }

    MapLayer& layer = layers[layerId];

    if (row < 0 || row >= static_cast<int>(layer.tiles.size()))
    {
        std::cerr << "Error: Row " << row << " is out of bounds." << std::endl;
        return nullptr;
    }

    std::vector<Tile>& tileRow = layer.tiles[row];

    if (col < 0 || col >= static_cast<int>(tileRow.size()))
    {
        std::cerr << "Error: Column " << col << " is out of bounds.\n" << "the tileRow.size() is " << tileRow.size() << std::endl;
        return nullptr;
    }

    return &tileRow[col];
}