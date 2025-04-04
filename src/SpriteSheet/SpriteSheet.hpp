#include "raylib.h"
#include <cstddef>
#include <iostream>

struct SpriteSheet
{
	int sizeX = 1;
	int sizeY = 1;

	float tileWidth;
    float tileHeight;
	int spriteCount;
	Texture2D texture;

	void drawSprite(int tileId, float posX, float posY) const;
    void init(const char *path, int tileWidth, int tileHeight);
};