#include "Game.h"

int main()
{
	// Create the game object
	Game* game = new Game();

	// Initialize and run the game
	if (game->Init())
	{
		game->Run();
	}

	// Clean up
	delete game;
	return 0;
}