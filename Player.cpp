#include "Player.h"
#include "UGFW.h"
#include "UG_Defines.h"
#include <iostream>
#include "Game.h"
#include "laser.h"
#include <Windows.h>
#include <vector>
#include <string>

Player::Player()
{
}

Player::~Player()
{
}

struct Position {
	//X and Y coordinates given to the player's fighter
	float X = 0;
	float Y = 0;
};

struct Fighter {
	//sprite ID and X/Y
	int iFighterSprite = 1;
	Position position;
};

struct LivesUI {
	//used for the life sprites in the bottom right
	//SpriteID and X/Y (X/Y doesnt change so it can be set here)
	int iLivesSprite = 0;
	float X = 700;
	float Y = 50;
};

//vector list of the life sprites
std::vector<LivesUI> lifebar;

Fighter fighter;
Laser laser;
Game reset;

//Game starts off without any shots being fired so this bool is set to false;
bool shotIsFired = false;
int Lives = 3;

void Player::Movement() {

	//Gets the player's fighter's current position, changes it in the x direction based on input
	UG::GetSpritePosition(fighter.iFighterSprite, fighter.position.X, fighter.position.Y);

	if (UG::IsKeyDown(UG::KEY_A)) {
		UG::SetSpritePosition(fighter.iFighterSprite, fighter.position.X - 5, fighter.position.Y);
	}

	if (UG::IsKeyDown(UG::KEY_D)) {
		UG::SetSpritePosition(fighter.iFighterSprite, fighter.position.X + 5, fighter.position.Y);

	}

	//Keeping true to the original game, the player can only have one shot on screen at a time, so if the player presses the shoot button (space) but there is an active shot, it wont fire
	if (UG::IsKeyDown(UG::KEY_SPACE)) {
		if (!laser.isAlive) {
			//shoots laser if there isnt one already active
			shootLaser();
			//Once again keeping true to the original game, UFO points depend on shots fired, so this is kept track of in here
			shotCount++;
		}
	}
}

void Player::shootLaser() {
	//Shoots the laser, passes in the players X/Y coordinates so the laser knows where to originate from
	laser.Shoot(fighter.position.X, fighter.position.Y);
}

void Player::Initialise() {
	//when the game is reset, the lifebar vector is cleared and is told to reserve 3 spaces for the lives, this avoids more lives being shown than had
	lifebar.clear();
	lifebar.reserve(3);
	Alive = true;
	for (int i = 0; i < Lives; i++) {
		std::cout << i << std::endl;
		//creates a life sprite for each life had and adds it to the lifebar vector
		LivesUI* life = new LivesUI();
		life->iLivesSprite = UG::CreateSprite("../images/fighter.png", (float)(16), (float)(16), true);
		//for each new life created, it is moved along from the previous to avoid them all being in the same spot
		UG::SetSpritePosition(life->iLivesSprite, life->X + (24 * i), life->Y);
		lifebar.push_back(*life);
	}
	//initialises the players fighter, kills any active lasers
	fighter.iFighterSprite = UG::CreateSprite("../images/fighter.png", 32.0f, 32.0f, true);
	UG::SetSpritePosition(fighter.iFighterSprite, 1024 / 2, 768 / 8);
	fighter.position.X = 1024 / 2;
	fighter.position.Y = 768 / 8;
	laser.isAlive = false;
}

void Player::DrawPlayer() {
	//Every frame lives are drawn, the player is moved, any current shots are moved and then the player is drawn
	DrawLives();
	Movement();
	laser.Movement();
	UG::DrawSprite(fighter.iFighterSprite);
}

void Player::DrawLives() {
	//draws a life for each one had
	for (int i = 0; i < Lives; i++) {
		UG::DrawSprite(lifebar[i].iLivesSprite);
	}
}

void Player::DetectHit(float X, float Y) {
	//This is called by the Invader's shots and their X/Y is passed in, if their X/Y is shared with the player it is a hit
	if (X > (fighter.position.X - 16) && X < (fighter.position.X + 16)) {
		if (Y > (fighter.position.Y - 16) && Y < (fighter.position.Y + 16)) {
			//Decrements the amount of lives held, if this causes there to be none left, it is a defeat and so the game is switched over to the defeat menu
			Lives--;
			std::cout << Lives << std::endl;
			if (Lives <= 0) {
				Alive = false;
			}
			//if there are still lives left after being hit, the scene is reset (points and lives are kept the same)
			UG::ClearScreen();
			reset.Reset();
			Sleep(1000);
		}
	}
}

int Player::GetLives() {
	return Lives;
}

void Player::SetLives() {
	Lives = 3;
}