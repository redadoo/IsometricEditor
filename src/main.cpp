#include "Game.hpp"

int main(void) 
{
	Game game = Game();
	
	game.init();
	game.loop();
	
	return 0;
}
