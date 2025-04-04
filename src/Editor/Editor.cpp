#include "Editor.hpp"
#include <filesystem>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

Editor::Editor() : isMapInit(false)
{
	guiSection = EditorState::MAIN_MENU;

	InitWindow(screenWidth, screenHeight, "IsometricEditorTest");
	SetTargetFPS(60);

	InitCamera();
	InitGuiElements();
}

Editor::~Editor() {}

void Editor::InitCamera()
{
	camera = {
		{ screenWidth / 2.0f, screenHeight / 2.0f },
		{ 0.0f, 0.0f },
		0.0f,
		1.0f
	};
}

void Editor::InitGuiElements()
{
	const float buttonWidth = screenWidth * 0.3f;
	const float buttonHeight = screenHeight * 0.08f;
	const float buttonX = (screenWidth - buttonWidth) / 2.0f;

	guiElements[0] = { buttonX, screenHeight * 0.2f, buttonWidth, buttonHeight };
	guiElements[1] = { buttonX, screenHeight * 0.3f, buttonWidth, buttonHeight };
	guiElements[2] = { 10, 10, 200, 50 };
	guiElements[3] = { 0, screenHeight - 100, screenWidth, 100 };

	inputText.labelRec   = { buttonX, screenHeight * 0.3f - 20, buttonWidth, 20 };
	inputText.textBoxRec = { buttonX, screenHeight * 0.3f, buttonWidth, buttonHeight };
}

void Editor::manageCamera(Vector2 mousePos)
{
	float wheel = GetMouseWheelMove();

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		Vector2 delta = GetMouseDelta();
		delta = Vector2Scale(delta, -1.0f / camera.zoom);
		camera.target = Vector2Add(camera.target, delta);
	}

	if (wheel != 0)
	{
		camera.offset = mousePos;
		camera.target = GetScreenToWorld2D(mousePos, camera);

		float scaleFactor = 1.0f + (0.25f * fabsf(wheel));
		if (wheel < 0)
			scaleFactor = 1.0f / scaleFactor;
		camera.zoom = Clamp(camera.zoom * scaleFactor, 0.125f, 64.0f);
	}
}

void Editor::drawMainMenu()
{
	if (GuiButton(guiElements[0], "new map"))
		guiSection = EditorState::MAP_EDITOR;

	if (GuiButton(guiElements[1], "Load Map"))
		guiSection = EditorState::SEARCH_MAP_PATH;
}

void Editor::drawInputPath()
{
	GuiLabel(inputText.labelRec, "Insert map path:");
	if (GuiTextBox(inputText.textBoxRec, inputText.path, sizeof(inputText.path), true))
	{
		if (std::filesystem::exists(std::filesystem::path(inputText.path)))
			guiSection = EditorState::MAP_EDITOR;
	}
}

void Editor::drawMapGui()
{
	DrawRectangleRec(guiElements[3], BLACK);

	for (size_t i = 0; i < 10; i++) {}
}

void Editor::ManageGui()
{
	if (GuiButton(guiElements[2], "Back to Main menu"))
		guiSection = EditorState::MAIN_MENU;

	switch (guiSection)
	{
	case EditorState::MAIN_MENU:
		drawMainMenu();
		break;
	case EditorState::SEARCH_MAP_PATH:
		drawInputPath();
		break;
	case EditorState::MAP_EDITOR:
		drawMapGui();
		break;
	default:
		break;
	}
}

void Editor::run()
{
	while (!WindowShouldClose())
	{
		if (!isMapInit && guiSection == EditorState::MAP_EDITOR)
		{
			mapManager.init(inputText.path, screenWidth);
			isMapInit = true;
		}

		Vector2 mousePos = GetMousePosition();
		manageCamera(mousePos);

		BeginDrawing();
		ClearBackground(GRAY);

		BeginMode2D(camera);
		mapManager.draw(mousePos);
		EndMode2D();

		ManageGui();
		EndDrawing();
	}
	CloseWindow();
}
