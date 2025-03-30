#include <string>

class Gui
{
private:
	bool showMap = false;
	bool closeMenu = false;
	bool showMessageBox = false;
	bool showMapCreationBox = false;
	bool showFilePathInputBox = false;

	float screenWidth = 1280;
	float screenHeight = 720;

	float buttonWidth = screenWidth * 0.3f;
	float buttonHeight = screenHeight * 0.08f;

	float buttonX = (screenWidth - buttonWidth) / 2;
	float buttonY1 = screenHeight * 0.2f;
	float buttonY2 = screenHeight * 0.3f;
	float buttonY3 = screenHeight * 0.4f;
public:
	Gui();
	~Gui();

	void draw();
};