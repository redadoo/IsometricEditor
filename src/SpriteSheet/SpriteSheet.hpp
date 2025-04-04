#pragma once

#include "raylib.h"
#include <cstddef>
#include <iostream>

/**
 * @brief A structure for handling sprite sheet operations.
 */
struct SpriteSheet {
	int sizeX = 1;
	int sizeY = 1;
	float tileWidth {0.0f};
	float tileHeight {0.0f};
	int spriteCount {0};
	Texture2D texture{};

	/**
	 * @brief Draws a sprite by its ID at the given position.
	 * 
	 * @param tileId The 1-indexed ID of the sprite tile to draw.
	 * @param posX The X-coordinate on the screen.
	 * @param posY The Y-coordinate on the screen.
	 */
	void drawSprite(int tileId, float posX, float posY) const;

	/**
	 * @brief Initializes the sprite sheet by loading an image from the provided path
	 * and setting up the tile dimensions.
	 * 
	 * @param path The file path to the sprite sheet image.
	 * @param tileWidth The width of each sprite tile.
	 * @param tileHeight The height of each sprite tile.
	 */
	void init(const char* path, int tileWidth, int tileHeight);
};
