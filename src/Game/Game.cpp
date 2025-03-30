#include "Game.hpp"

void Game::init()
{
	InitWindow(this->screenWidth, this->screenHeight, "IsometricTest");
	
	this->mapManager = MapManager();
	this->mapManager.initMap("assets/map/map.json");

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
		float wheel = GetMouseWheelMove();
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }
		if (wheel != 0)
		{
			Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
			camera.offset = GetMousePosition();
			camera.target = mouseWorldPos;

			float scaleFactor = 1.0f + (0.25f*fabsf(wheel));
			if (wheel < 0) 
				scaleFactor = 1.0f/scaleFactor;
			camera.zoom = Clamp(camera.zoom*scaleFactor, 0.125f, 64.0f);
		}
		
		BeginDrawing();
		ClearBackground(WHITE);
		
		gui.draw();
		
		BeginMode2D(this->camera);
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