#include "SpriteSheet.hpp"

void SpriteSheet::drawSprite(int tileId, float posX, float posY)
{
    if (tileId <= 0)
        return;

    int tileIndex = tileId - 1; 
    int tileSpritecol = tileIndex % sizeX;
    int tileSpriterow = tileIndex / sizeY;
    float y = tileSpriterow * tileWidth;
    float x = tileSpritecol * tileHeight;
    Rectangle rec = { x, y, tileWidth, tileHeight };
    DrawTextureRec(this->texture, rec, {posX, posY}, WHITE);
}

void SpriteSheet::init(const char *path, int tileWidth, int tileHeight)
{
	Image spriteImage = LoadImage(path);
	
	if (spriteImage.data == NULL)
	{
		std::cerr << "cant load SpriteSheet from image path " << path << "\n";
		return;
	}

	this->texture = LoadTextureFromImage(spriteImage);
	this->sizeX = spriteImage.width / tileWidth;
	this->sizeY = spriteImage.height / tileHeight;
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;
	UnloadImage(spriteImage);
}
