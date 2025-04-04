#pragma once

#include <array>
#include <filesystem>
#include "raylib.h"
#include "raymath.h"
#include "raygui.h"
#include "MapManager.hpp"

/**
 * @brief Enumeration for the different GUI sections.
 */
enum class EditorState { MAIN_MENU, SEARCH_MAP_PATH, MAP_EDITOR };

/**
 * @brief Structure for handling text input for map paths.
 */
struct InputText {
	Rectangle labelRec;
	Rectangle textBoxRec;
	char path[256] = "assets/map/map2.json";
};

/**
 * @brief Editor class manages the application state and rendering.
 */
class Editor {
public:
	Editor();
	~Editor();

	/**
	 * @brief Runs the main loop of the editor application.
	 */
	void run();

private:
	static constexpr float screenWidth = 1280.0f;
	static constexpr float screenHeight = 720.0f;

	EditorState guiSection;
	InputText inputText;
	bool isMapInit = false;
	std::array<Rectangle, 4> guiElements;
	MapManager mapManager;
	Camera2D camera;

	/**
	 * @brief Initializes the camera settings.
	 */
	void InitCamera();

	/**
	 * @brief Initializes the GUI elements.
	 */
	void InitGuiElements();

	/**
	 * @brief Updates the camera based on the current mouse position.
	 * @param mousePos The current position of the mouse.
	 */
	void manageCamera(Vector2 mousePos);

	/**
	 * @brief Manage the entire GUI based on the current section.
	 */
	void ManageGui();

	/**
	 * @brief Draws the main menu interface.
	 */
	void drawMainMenu();

	/**
	 * @brief Draws the interface for inputting a map path.
	 */
	void drawInputPath();

	/**
	 * @brief Draws the GUI for the map editor.
	 */
	void drawMapGui();
};
