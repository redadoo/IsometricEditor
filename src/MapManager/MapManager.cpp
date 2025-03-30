#include "MapManager.hpp"

MapManager::MapManager()
{
	this->tilesSprite = std::map<int, Texture2D>();
}

MapManager::~MapManager()
{
	serializeMap();
}

void MapManager::initMap(const char *pathMap)
{
	if (pathMap != nullptr)
		this->map = deSerializeMap(pathMap);
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

Map MapManager::deSerializeMap(const char *pathMap)
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

    // Load the tileset texture
    std::string tileSheetSource = m["tilesets"][0]["source"].get<std::string>();
    loadTexture("atlas", tileSheetSource.c_str(), true);

    for (auto& layerJson : m["layers"]) 
	{
		MapLayer layer;
		layer.id = layerJson["id"].get<int>();
		
		// Check if 'data' is a 2D array
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

void MapManager::loadTexture(const char *key, const char *path, bool isSpriteSheet)
{
	Image spriteImage = LoadImage(path);
	if (spriteImage.data != NULL)
	{
		Texture2D sprite = LoadTextureFromImage(spriteImage);
		UnloadImage(spriteImage); 
		SpriteSheet spriteSheet;
		spriteSheet.texture = sprite;

		tilesSpriteSheet.insert( {0, spriteSheet} );
	}
	else
		std::cout << "error\n" ;
}

void MapManager::drawMap()
{
	for (size_t layerIndex = 0; layerIndex < map.layers.size(); ++layerIndex) 
	{
		const auto& layer = map.layers[layerIndex];
		for (int i = 0; i < this->map.width; i++) 
		{
			for (int j = 0; j < this->map.height; j++) 
			{
				Tile tile = layer.tiles[i][j];
				SpriteSheet sprite = this->tilesSpriteSheet[0];
				
				int isoX = (i - j) * (map.tileWidth / 2.5f);
				int isoY = (i + j) * (map.tileHeight / 2.5f);

				sprite.drawSprite(tile, isoX, isoY);
			}
		}
	}
}

void SpriteSheet::drawSprite(Rectangle rec, Vector2 pos)
{
	DrawTextureRec(this->texture, rec, pos, WHITE);
}

void SpriteSheet::drawSprite(const Tile& tile, int isoX, int isoY)
{
	int x = tile.id * 32;
	int y = ((int)tile.id / 10) * 32;
	Rectangle rec = { x, y, 32, 32 };
	DrawTextureRec(this->texture, rec, {isoX, isoY}, WHITE);
}
