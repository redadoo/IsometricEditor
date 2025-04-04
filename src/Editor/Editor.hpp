#pragma once

#include <array>
#include <filesystem>
#include "raylib.h"
#include "raymath.h"
#include "raygui.h"
#include "MapManager.hpp"

enum class GuiSection { MAIN_MENU, SEARCH_MAP_PATH, MAP_EDITOR };

struct InputText {
	Rectangle labelRec;
	Rectangle textBoxRec;
	char path[256] = "assets/map/map2.json";
};

class Editor {
public:
	Editor();
	~Editor();

	void run();

private:
	static constexpr float screenWidth = 1280.0f;
	static constexpr float screenHeight = 720.0f;

	GuiSection guiSection;
	InputText inputText;
	bool isMapInit = false;
	std::array<Rectangle, 4> guiElements;
	MapManager mapManager;
	Camera2D camera;

	void InitCamera();
	void InitGuiElements();
	void manageCamera(Vector2 mousePos);
	void drawGui();
	void drawMainMenu();
	void drawInputPath();
	void drawMapGui();
};
