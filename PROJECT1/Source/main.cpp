// processing events
#include "EventHandler.h"
#include "../Graphics.h"
#include "Gameplay/GameplayManager.h"

// argc and argv here are command line parameters passed to the program when running the program. 
// we won't actually use them but our 2D Graphics Library requires the main setup like this.
// Argc: the number of arguments pass
// argv[] : a char array of each of the params passed in ( each space is a new param)
int main( int argc, char *argv[] )
{
	if (!Graphics::Init())
	{
		return false;
	}

	GameplayManager Game;

	// Main game loop
	while (Game.run) 
	{
		// handle button events
		Game.run = EventHandler::Update();
		Game.Update();

		Graphics::StartRender();

		Game.Render();

		SDL_Delay(8); // around 125 fps 

		// apply the changes to the screen 
		Graphics::EndRender();
	}

	//close off the SDL window
	SDL_Quit();

	return true;
}
