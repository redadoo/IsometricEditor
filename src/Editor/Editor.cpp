#include "Editor.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

Editor::Editor()
{
	this->guiSection = GuiSection::MAIN_MENU;

	float buttonWidth = screenWidth * 0.3f;
	float buttonHeight = screenHeight * 0.08f;
	float buttonX = (screenWidth - buttonWidth) / 2;

    Rectangle newMapBtn = { buttonX, screenHeight * 0.2f, buttonWidth, buttonHeight };
    Rectangle loadMapBtn = { buttonX, screenHeight * 0.3f, buttonWidth, buttonHeight };
	Rectangle btnBack = { 10, 10, 200, 50 };
	Rectangle toolbarRec = {0, screenHeight - 100, screenWidth, 100};

	guiElements[0] = newMapBtn;
	guiElements[1] = loadMapBtn;
	guiElements[2] = btnBack;
	guiElements[3] = toolbarRec;

	this->inputText.labelRec =  { buttonX, screenHeight * 0.3f - 20, buttonWidth, 20 };
	this->inputText.textBoxRec = { buttonX, screenHeight * 0.3f, buttonWidth, buttonHeight };

	InitWindow(this->screenWidth, this->screenHeight, "IsometricEditorTest");

	this->camera = { 
		{ this->screenWidth / 2.0f, this->screenHeight / 2.0f}, 
		{ 0.0f, 0.0f }, 
		0.0f, 
		1.0f 
	};

	SetTargetFPS(60);
}

Editor::~Editor() {}

void Editor::manageCamera(Vector2 mousePos)
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
		camera.offset = mousePos;
		camera.target = GetScreenToWorld2D(mousePos, camera);

		float scaleFactor = 1.0f + (0.25f*fabsf(wheel));
		if (wheel < 0) 
			scaleFactor = 1.0f/scaleFactor;
		camera.zoom = Clamp(camera.zoom * scaleFactor, 0.125f, 64.0f);
	}

}

void Editor::drawMainMenu()
{
	if (GuiButton(guiElements[0], "new map"))
		this->guiSection = GuiSection::MAP_EDITOR;

	if (GuiButton(guiElements[1], "Load Map"))
		this->guiSection = GuiSection::SEARCH_MAP_PATH;
}

void Editor::drawInputPath()
{
	GuiLabel(this->inputText.labelRec, "Insert map path:");
	if (GuiTextBox(this->inputText.textBoxRec, this->inputText.path, sizeof(this->inputText.path), true))
	{
		if (std::filesystem::exists(std::filesystem::path(this->inputText.path)))
			this->guiSection = GuiSection::MAP_EDITOR;
	}
}

void Editor::drawMapGui()
{
	DrawRectangleRec(guiElements[3], BLACK);

	for (size_t i = 0; i < 10; i++)
	{
		// int tileSpritecol = i % 32;
		// int tileSpriterow = i / 32;
		// float y = tileSpriterow * 32;
		// float x = tileSpritecol * 32;
		// Rectangle rec = { x, y, 32, 32 };
		// float t = i * 32;
		// Rectangle itemRec = {t, screenHeight - 100, 32, 100};

		// DrawTextureRec(toolbarItemsSpriteSheet, rec, (Vector2){itemRec.x, itemRec.y}, WHITE);
	
		// if (CheckCollisionPointRec(GetMousePosition(), itemRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
		// {
		// 	selectedItem = i;
		// }
	
		// Vector2 start = {itemRec.x + itemRec.width, itemRec.y};
		// Vector2 end = {itemRec.x + itemRec.width, itemRec.y + itemRec.height};
		// DrawLine(start.x, start.y, end.x, end.y, LIGHTGRAY);		
	}
}

void Editor::drawGui()
{
	if (GuiButton(guiElements[2], "Back to Main menu"))
		this->guiSection = GuiSection::MAIN_MENU;

	switch (this->guiSection)
	{
		case GuiSection::MAIN_MENU:
			this->drawMainMenu();
			break;
		case GuiSection::SEARCH_MAP_PATH:
			this->drawInputPath();
			break;
		case GuiSection::MAP_EDITOR:
			this->drawMapGui();
			break;
		default:
			break;
	}
}

void Editor::run()
{
	while (!WindowShouldClose()) 
	{
		if (this->isMapInit == false && this->guiSection == GuiSection::MAP_EDITOR)
		{
			this->mapManager.init(this->inputText.path, this->screenWidth);
			this->isMapInit = true;
		}

		Vector2 mousePos = GetMousePosition();

		this->manageCamera(mousePos);

		BeginDrawing();
		ClearBackground(GRAY);

		BeginMode2D(this->camera);
		this->mapManager.draw(mousePos);
		EndMode2D();

		this->drawGui();
		EndDrawing();
	}
	CloseWindow();
}