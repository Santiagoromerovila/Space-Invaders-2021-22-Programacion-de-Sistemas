#include "UFO.h"
#include "UGFW.h"
#include "UG_Defines.h"
#include <stdlib.h>
#include <iostream>
#include "player.h"
#include "Game.h"
#include <math.h>

UFO::UFO()
{
}

UFO::~UFO()
{
}

Player playerShots;
Game game;

struct SpaceShip {
	//the UFO has a sprite ID and X/Y
	int iUFOSprite = 1;
	float X = 1088;
	float Y = 700;
};

SpaceShip spaceship;

void UFO::Spawn() {
	//This is called each frame from the game script
	if (!onScreen) {
		//a cheap frame wait to decrease the rate of UFOs spawning
		frameCount++;
		if (frameCount > 5) {
			//The chance of a UFO spawning is just random
			int random = rand() % 300;
			if (random == 55) {
				onScreen = true;
			}
			frameCount = 0;
		}
	}
	if (onScreen) {
		//UFO is moved then drawn
		Movement();
		UG::DrawSprite(spaceship.iUFOSprite);
	}
}

void UFO::Movement() {
	//No fancy movement styles for the UFO, it just pans across the screen at a constant rate and disappears once its off screen
	spaceship.X -= 5.0f;
	UG::SetSpritePosition(spaceship.iUFOSprite, spaceship.X, 700);
	if (spaceship.X <= -64) {
		onScreen = false;
		//X position is reset for the next UFO
		spaceship.X = 1088;
	}
}

bool UFO::HitDetection(float shotX, float shotY) {
	//Called by the player's laser - Checks to see if its X/Y is shared by the laser (a hit)
	if (shotY > spaceship.Y - 32 && shotY < spaceship.Y + 32) {
		if (shotX > spaceship.X - 64 && shotX < spaceship.X + 64) {
			//staying with the original game, the points from the UFO depend on the number of shots fired - 200, or 300 on the 23rd shot and every 15 after that
			if (playerShots.shotCount == 23 || (remainder((playerShots.shotCount - 23), 15)) == 0) {
				game.ScoreHolder(300);
			}
			else {
				game.ScoreHolder(200);
			}
			//UFO is killed and X is reset for the next UFO, laser is returned true so it knows it hit something and can destroy itself
			onScreen = false;
			spaceship.X = 1088;
			return true;
		}
	}
	//if the UFO is not hit, false is returned so that the laser can continue on
	return false;
}

void UFO::Initialise() {
	//Initialises the UFO's Sprite and X/Y, it isnt on screen so onScreen is false
	spaceship.iUFOSprite = UG::CreateSprite("../images/UFO.png", (float)(64), (float)(32), true);
	UG::SetSpritePosition(spaceship.iUFOSprite, spaceship.X, spaceship.Y);
	onScreen = false;
	spaceship.X = 1088;
}

int UFO::Score() {
	return iUFOScore;
}