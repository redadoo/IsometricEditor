#include "Game.hpp"

void Game::init()
{
	
	InitWindow(this->screenWidth, this->screenHeight, "IsometricTest");
	
	this->mapManager = MapManager();

	this->mapManager.loadTexture("atlas", "assets/tile/spritesheet.png", true);
	// this->mapManager.loadTexture("dirt", "assets/tile/dirt_0.png", false);

	this->mapManager.initMap("assets/map/test.json");
	
	SetTargetFPS(60);

	this->camera = 
	{ 
		{ this->screenWidth / 2.0f, this->screenHeight / 2.0f}, 
		{ 0.0f, 0.0f }, 
		0.0f, 
		1.0f 
	};
}

void Game::loop()
{
	while (!WindowShouldClose()) 
	{
		
		// if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
		// {
		// 	Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
		// 	camera.offset = GetMousePosition();
		// 	camera.target = mouseWorldPos;
		// }
		// if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
		// {
		// 	float deltaX = GetMouseDelta().x;
		// 	float scaleFactor = 1.0f + (0.01f * fabsf(deltaX));
		// 	if (deltaX < 0) scaleFactor = 1.0f / scaleFactor;
		// 	camera.zoom = Clamp(camera.zoom * scaleFactor, 0.125f, 64.0f);
		// }
		
		BeginDrawing();
		ClearBackground(WHITE);
		BeginMode2D(camera);

		this->mapManager.drawMap();

		EndMode2D();
		EndDrawing();
	}
	CloseWindow();
}

Game::Game()
{
	
}

Game::~Game()
{
}