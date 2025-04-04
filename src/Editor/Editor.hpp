#include "raylib.h"
#include "raymath.h"
#include "raygui.h" 
#include "math.h"
#include "stdio.h"
#include <filesystem>
#include "MapManager.hpp"
#include <array>

enum class GuiSection { MAIN_MENU, SEARCH_MAP_PATH, MAP_EDITOR };

struct InputText
{
	Rectangle labelRec;
	Rectangle textBoxRec;

	char path[256] = "assets/map/map2.json";
};

class Editor
{
private:
	GuiSection guiSection;
	InputText inputText;
	const float screenWidth = 1280;
	const float screenHeight = 720;
	bool isMapInit = false;
	std::array<Rectangle, 4> guiElements;
	MapManager mapManager;
	Camera2D camera;
	
	void manageCamera(Vector2 mousePos);
    void drawGui();
	void drawMainMenu();
    void drawInputPath();
    void drawMapGui();
public:
	void run();

	Editor();
	~Editor();
};
