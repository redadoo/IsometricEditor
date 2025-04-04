#include "SpriteSheet.hpp"
#include <iostream>

void SpriteSheet::drawSprite(int tileId, float posX, float posY) const 
{
	if (tileId <= 0)
		return;

	int tileIndex = tileId - 1;
	int tileCol = tileIndex % sizeX;
	int tileRow = tileIndex / sizeX;
	float x = tileCol * tileWidth;
	float y = tileRow * tileHeight;
	Rectangle srcRect = { x, y, tileWidth, tileHeight };

	DrawTextureRec(texture, srcRect, { posX, posY }, WHITE);
}

void SpriteSheet::init(const char *path, int tileWidth, int tileHeight) 
{
	Image spriteImage = LoadImage(path);

	if (!spriteImage.data) 
	{
		std::cerr << "Error: Unable to load SpriteSheet from image path " << path << "\n";
		return;
	}

	texture = LoadTextureFromImage(spriteImage);
	sizeX = spriteImage.width / tileWidth;
	sizeY = spriteImage.height / tileHeight;
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;

	UnloadImage(spriteImage);
}
