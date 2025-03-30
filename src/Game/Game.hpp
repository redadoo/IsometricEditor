#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include "stdio.h"
#include "MapManager.hpp"
#include "Gui.hpp"

class Game
{
private:
	const int screenWidth = 1280;
	const int screenHeight = 720;
	MapManager mapManager;
	Camera2D camera;
	Gui gui;
	
public:
	void init();
	void loop();

	Game();
	~Game();
};
