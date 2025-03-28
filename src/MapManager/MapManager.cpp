#include "MapManager.hpp"

MapManager::MapManager()
{
    this->tilesSprite = std::map<const char*, Texture2D>();
}

MapManager::~MapManager()
{

}

void MapManager::loadTexture(const char* key, const char* path, bool isSpriteSheet)
{
    Image spriteImage = LoadImage(path);
	if (spriteImage.data == NULL)
        std::cout << "error\n" ;
    else
    {
        Texture2D sprite = LoadTextureFromImage(spriteImage);
        UnloadImage(spriteImage); 
        if (!isSpriteSheet)
            tilesSprite.insert( {key, sprite} );
        else
            tilesSpriteSheet.insert( {key, sprite} );
    }
}

void MapManager::initMap(const char *pathMap)
{
    if (pathMap == nullptr)
    {
        this->map = Map();
    }
    else
    {
        
    }
}

void MapManager::drawMap()
{
    for (int i = 0; i < this->map.xSize; i++) 
    {
        for (int j = 0; j < this->map.ySize; j++) 
        {
            // Tile sprite = this->map.mapMatrix[i][j];

            // int isoX = (i - j) * (sprite.tileWidth / 2);
            // int isoY = (i + j) * (sprite.tileHeight / 2);

            // DrawTexture(sprite.sprite, isoX + i * 3, isoY + j * 2, WHITE);
        }
    }

}
