#include "Game.hpp"

Game::Game()
{
	InitWindow(this->screenWidth, this->screenHeight, "IsometricTest");
	this->mapManager.init("assets/map/map.json", this->screenWidth);
	this->camera = { 
		{ this->screenWidth / 2.0f, this->screenHeight / 2.0f}, 
		{ 0.0f, 0.0f }, 
		0.0f, 
		1.0f 
	};

	SetTargetFPS(60);
}

Game::~Game() {}

void Game::manageCamera()
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
}

void Game::run()
{
	while (!WindowShouldClose()) 
	{
		this->manageCamera();

		BeginDrawing();
		ClearBackground(WHITE);
		BeginMode2D(this->camera);

		this->mapManager.draw();
 		
		EndMode2D();
		EndDrawing();
	}
	CloseWindow();
}