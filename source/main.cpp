//\===========================================================================================================================================
//\ Filename: Space Invaders
//\ Author  : Aren Dyer
//\ Brief   : 4019 Assessment 1 - Retro 2D Arcade Game
//\===========================================================================================================================================

#include "UGFW.h"
#include "UG_Defines.h"
#include "Game.h"
#include "Player.h"
#include <iostream>
#include <windows.h>

int main(int argv, char* argc[])
{
	Game game;

	//initialises the game and returns a bool of true if it was compiled without error
	bool bSuccess = game.Initialise();
	if (bSuccess) {
		bool bStillRunning = true;
		while (bStillRunning) {
			//Until the player Decides to quit the game, the game will continue to update
			//After every frame a bool is sent back saying whether the player is still playing or not
			//a sleep() has been called after every frame just to slow the game down a bit
			bStillRunning = game.Update();
			Sleep(50);
		}
	}

	//Once the player has stopped playing, The game will Deinitialise which will Dispose of the used memory and quit the application
	game.Deinitialise();
}