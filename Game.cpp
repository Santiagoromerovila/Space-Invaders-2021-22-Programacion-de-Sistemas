#include "Game.h"
#include "UGFW.h"
#include "UG_Defines.h"
#include "Player.h"
#include "Invader.h"
#include <iostream>
#include <Windows.h>
#include "UFO.h"
#include "Barricades.h"
#include <string>

Player player;
Invader invaders;
UFO ufoSpawner;
Barricades base;

Game::Game()
{
}


Game::~Game()
{
}

int score = 0;

bool Game::Initialise() {

	//If there is an error in the framework creating the window, it will return a success bool of false
	bool bSuccess = UG::Create(1024, 768, false, "Space Invaders", 100, 100);
	if (bSuccess) {
		m_eCurrentGameState = MENU;
		//Initialises the menu screen sprites and sets the background colour to black
		UG::SetBackgroundColor(UG::SColour(0x00, 0x00, 0x00, 0xFF));
		menu = UG::CreateSprite("../images/Menu.png", (float)(1024), (float)(768), true);
		UG::SetSpritePosition(menu, 512, 384);
		pauseScreen = UG::CreateSprite("../images/Pause.png", (float)(1024), (float)(768), true);
		UG::SetSpritePosition(pauseScreen, 512, 384);
		defeatScreen = UG::CreateSprite("../images/Defeat.png", (float)(1024), (float)(768), true);
		UG::SetSpritePosition(defeatScreen, 512, 384);
		//A 'Retro Style' Font has been set for showing the player their score
		int m_Font = UG::AddFont("../fonts/vcr.ttf", 30);
		UG::SetFont(m_Font);
	}
	return bSuccess;
}

bool Game::Update() {

	//Clears the screen before each frame so that there arent any residual sprites clogging up the screen
	UG::ClearScreen();

	switch (m_eCurrentGameState)
	{
	case MENU:
	{
		//Draws the Menu screen, if the player presses the enter Key, the game will start, if they press the escape key the game will deinitialse
		UG::DrawSprite(menu);
		if (UG::IsKeyDown(UG::KEY_ENTER)) {
			m_eCurrentGameState = PLAY;
		}
		if (UG::IsKeyDown(UG::KEY_ESCAPE)) {
			m_eCurrentGameState = EXIT;
		}
		break;
	}
	case PLAY:
	{
		if (!gameStarted) {
			//Initialises all the sprites if the game hasnt already started. when the player loses a life or the wave is cleared, 
			//this is called again to reset the screen (the score and lives are still left in tact)
			player.Initialise();
			base.Initialise();
			ufoSpawner.Initialise();
			invaders.Initialise();
			gameStarted = true;
		}
		// this is the main game logic where movement, input and drawing is kept
		Draw();

		//the player can pause the game at any time using the escape key
		if (UG::IsKeyDown(UG::KEY_ESCAPE)) {
			m_eCurrentGameState = PAUSE;
		}
		break;
	}
	case PAUSE: {
		//Sleep() as to stop the game imediatley exiting if the player holds down the key for too long
		Sleep(1000);
		//Draws the pause screen, the player can resume with the Enter key or Quit with escape
		UG::DrawSprite(pauseScreen);
		if (UG::IsKeyDown(UG::KEY_ENTER)) {
			m_eCurrentGameState = PLAY;
		}
		if (UG::IsKeyDown(UG::KEY_ESCAPE)) {
			m_eCurrentGameState = EXIT;
		}
		break;
	}
	case DEFEAT: {
		//Draws defeat screen and shows the player their score. the player can Play again with Enter or exit with Escape
		UG::DrawSprite(defeatScreen);
		std::string DefeatText = "YOU SCORED : " + std::to_string(score);
		//UG::DrawString(DefeatText.c_str(), 350, 384, 0);
		Sleep(300);
		if (UG::IsKeyDown(UG::KEY_ENTER)) {
			player.SetLives();
			score = 0;
			gameStarted = false;
			m_eCurrentGameState = MENU;
		}
		if (UG::IsKeyDown(UG::KEY_ESCAPE)) {
			m_eCurrentGameState = EXIT;
		}
		break;
	}
	case EXIT:
	{
		//closes the game
		UG::Close();
		break;
	}
	}

	//returned to main, if the game was stopped, false is returned and the memory is disposed of. otherwise true is returned and the game keeps playing
	bool bStillRunning = UG::Process();
	return bStillRunning;
}

void Game::Reset() {
	player.Initialise();
	base.Initialise();
	ufoSpawner.Initialise();
	invaders.Initialise();
}

void Game::Draw() {
	//Draws all the in-game sprites every frame (the UFO is given it's chance to spawn, it is only drawn once spawned), shows the player their score in the bottom left hand corner
	//DrawScore();
	player.DrawPlayer();
	ufoSpawner.Spawn();
	base.DrawBarricades();
	invaders.DrawInvaders();
	if (player.GetLives() <= 0 || invaders.GetLineCount() >= 11) {
		m_eCurrentGameState = DEFEAT;
		Sleep(1000);
	}

}

void Game::DrawScore() {
	std::string Text = ("SCORE : " + std::to_string(score));
	const char* ScoreText = Text.c_str();
	UG::DrawString(ScoreText, 50, 50, 1, UG::SColour(0xFF, 0xFF, 0xFF, 0xFF));
}

void Game::ScoreHolder(int points) {
	score += points;
}

void Game::Deinitialise() {
	//called when the game is quit, disposes of used memory
	UG::Dispose();
}