#include "Invader.h"
#include "Player.h"
#include "UGFW.h"
#include "UG_Defines.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "Game.h"
#include "Barricades.h"


const int iMaxInvaderCount = 55;
const float fSpriteSize = 32;

Game gamePoints;
Player playerPos;
Barricades barriers;


Invader::Invader()
{
}


Invader::~Invader()
{
}

struct Position {
	// X/Y for each Invader
	float X = 0;
	float Y = 0;
};

struct Invaders {
	//Each Invader has a Sprite ID, X/Y and point value
	int iInvadersSprite;
	Position position;
	int iPointValue;
};

struct Shot {
	//Each Shot has a Sprite ID, position and an Alive/Dead Indicator
	int iShotSprite = UG::CreateSprite("../images/laser.png", (float)(4), (float)(16));;
	float X;
	float Y;
	bool alive = false;
};

//vector lists to hold all the Invaders and Shots
std::vector<Invaders> InvaderList;
std::vector<Shot> ShotList;

void Invader::Initialise() {
	//Current Line the invaders are on - If they go past 11 the game is over
	lineCount = 0;
	//direction of travel | 1 = Left to Right | -1 = Right to Left
	direction = 1;
	iShotCount = 0;
	//Clears the lists and reserves the appropriate size - stops there being more than there should be in the event of a reset
	ShotList.clear();
	InvaderList.clear();
	InvaderList.reserve(iMaxInvaderCount);
	ShotList.reserve(3);
	for (int i = 0; i < 11; i++) {
		//Top row of invaders are initialised - The 'Squids' or 'Cuttlefish' these have a point value of 40
		Invaders* invader = new Invaders();
		invader->iInvadersSprite = UG::CreateSprite("../images/Squid.png", (float)(fSpriteSize), (float)(fSpriteSize), true);
		//Each new invader in the row is moved along from the other one to avoid them clumping together
		UG::SetSpritePosition(invader->iInvadersSprite, ((1024 / ((fSpriteSize + 1) * 11))) + ((fSpriteSize + 11) * i) + (fSpriteSize * 0.75f), 675 - (fSpriteSize * 0.9f));
		invader->position.X = ((1024 / ((fSpriteSize + 1) * 11))) + ((fSpriteSize + 11) * i) + (fSpriteSize * 0.75f);
		invader->position.Y = 675 - (fSpriteSize * 0.9f);
		invader->iPointValue = 40;
		//Each new invader is added the list
		InvaderList.push_back(*invader);
	}
	//in binary counting, these are the 2nd and 3rd rows
	//each new row is moved down By a multiple of j so that the 2nd row is one row down from the first for example
	for (int j = 1; j < 3; j++) {
		for (int k = 0; k < 11; k++) {
			//The same thing happens for the next 2 rows of 'Crabs' - these have a point value of 20
			Invaders* invader = new Invaders();
			invader->iInvadersSprite = UG::CreateSprite("../images/Crab.png", (float)(fSpriteSize), (float)(fSpriteSize), true);
			UG::SetSpritePosition(invader->iInvadersSprite, ((1024 / ((fSpriteSize + 1) * 11))) + ((fSpriteSize + 11) * k) + (fSpriteSize * 0.75f), 675 - (fSpriteSize * j) - (fSpriteSize * 0.9f));
			invader->position.X = ((1024 / ((fSpriteSize + 1) * 11))) + ((fSpriteSize + 11) * k) + (fSpriteSize * 0.75f);
			invader->position.Y = 675 - (fSpriteSize * j) - (fSpriteSize * 0.9f);
			invader->iPointValue = 20;
			InvaderList.push_back(*invader);
		}
	}
	//in binary counting, these are the 4th and 5th rows
	for (int l = 3; l < 5; l++) {
		for (int m = 0; m < 11; m++) {
			//The same thing happens for the next 2 rows of 'Octopuses' - these are the closest invaders to the player and so have a point value of 10
			Invaders* invader = new Invaders();
			invader->iInvadersSprite = UG::CreateSprite("../images/Octopus.png", (float)(fSpriteSize), (float)(fSpriteSize), true);
			UG::SetSpritePosition(invader->iInvadersSprite, ((1024 / ((fSpriteSize + 1) * 11))) + ((fSpriteSize + 11) * m) + (fSpriteSize * 0.75f), 675 - (fSpriteSize * l) - (fSpriteSize * 0.9f));
			invader->position.X = ((1024 / ((fSpriteSize + 1) * 11))) + ((fSpriteSize + 11) * m) + (fSpriteSize * 0.75f);
			invader->position.Y = 675 - (fSpriteSize * l) - (fSpriteSize * 0.9f);
			invader->iPointValue = 10;
			InvaderList.push_back(*invader);
		}
	}
}

void Invader::DrawInvaders() {

	frameCount++;
	//Does each movement only after so many frames as to get that 'jumping' motion whilst still keeping the player and laser movement smooth
	if (frameCount > 5) {
		frameCount = 0;
		Movement();
		if (lineCount > 11) {
			//once the invaders pass the 11th line it is a defeat
			DefeatState = true;
		}
	}
	//Moves the Invaders and the gives them a chance to shoot
	if (iShotCount < 3) {
		Shoot();
	}
	if (iShotCount > 0) {
		//For any shots alive, they are drawn then moved (the other way round would cause a vector error once they have been destroyed)
		for (int i = 0; i < iShotCount; i++) {
			if (ShotList[i].alive) {
				UG::DrawSprite(ShotList[i].iShotSprite);
				ShotMovement(i);
			}
		}
	}
	if (InvaderList.size() == 0) {
		//resets the scene (keeping points and lives intact) if there are no more invaders left
		gamePoints.Reset();
	}
	for (int j = 0; j < InvaderList.size(); j++) {
		//Draws each invader
		UG::DrawSprite(InvaderList[j].iInvadersSprite);
	}
}

void Invader::Movement() {
	//keeping true to the original game, the speed is increased for each invader destroyed
	//speed is the multiplier for the distance moved
	speed = 1.0f + ((iMaxInvaderCount - InvaderList.size()) / 15);
	for (int i = 0; i < InvaderList.size(); i++) {
		Invaders curXInv = InvaderList[i];
		UG::GetSpritePosition(curXInv.iInvadersSprite, curXInv.position.X, curXInv.position.Y);
		if (curXInv.position.X > 1024 || curXInv.position.X < 0) {
			//if the invader is off the screen, all invaders are dropped a row and the direction is reversed
			//to keep all the invaders together, the function then breaks as to start again in the next frame
			direction *= -1;
			lineCount++;
			for (int j = 0; j < InvaderList.size(); j++) {
				//drops the invaders Y position by its height, takes a step back in the new direction so it is back on screen again
				UG::GetSpritePosition(InvaderList[j].iInvadersSprite, InvaderList[j].position.X, InvaderList[j].position.Y);
				UG::SetSpritePosition(InvaderList[j].iInvadersSprite, InvaderList[j].position.X + ((fSpriteSize / 6) * direction * speed), InvaderList[j].position.Y -= fSpriteSize);
				//invaders have dropped a line so the line count is increased
			}
			break;
		}
		//if all the invaders are still on screen, then they just continue to travel in their original direction
		UG::SetSpritePosition(curXInv.iInvadersSprite, (curXInv.position.X += ((fSpriteSize / 6) * direction * speed)), curXInv.position.Y);
		InvaderList[i] = curXInv;
	}
}


bool Invader::DetectHit(float X, float Y) {
	//Called by the player's laser, with its X/Y passed in
	for (int i = 0; i < InvaderList.size(); i++) {
		Invaders curInv = InvaderList[i];
		//Each invader checks to see if it's X/Y is shared by the laser
		if (X > (curInv.position.X - fSpriteSize * 0.5) && X < (curInv.position.X + fSpriteSize * 0.5)) {
			if (Y > (curInv.position.Y - fSpriteSize * 0.5) && Y < (curInv.position.Y + fSpriteSize * 0.5)) {
				//If the Invader is hit, it is destroyed and removed from the vector, points are increased by the amount of points it holds
				gamePoints.ScoreHolder(curInv.iPointValue);
				UG::DestroySprite(curInv.iInvadersSprite);
				InvaderList.erase(InvaderList.begin() + i);
				//returns true to the laser so that it knows its hit something and can be destroyed
				return true;
				//breaks out of the loop as the laser has been destroyed and so no more invaders need to be checked
				break;
			}
		}
	}
	//if no invaders are hit, false is returned to the laser so it can continue travelling upwards
	return false;
}

void Invader::Shoot() {
	for (int i = 0; i < InvaderList.size(); i++) {
		//Each invader has the chance to shoot
		if (iShotCount < 3) {
			//Keeping true to the original game, the invader team can only have 3 shots on screen at once
				//the chance that the invader actually shoots is just a random number
			int random = rand() % 150;
			if (random == 24) {
				//shot count goes up so that no more than 3 shots are active at once
				iShotCount++;
				//A new shot is created with a sprite, X/Y (that of the invader it came from) and is made alive
				//This new shot is then added to the vector of shots
				Shot* shot = new Shot();
				shot->iShotSprite = UG::CreateSprite("../images/laser.png", (float)(4), (float)(16));
				UG::SetSpritePosition(shot->iShotSprite, InvaderList[i].position.X, InvaderList[i].position.Y - (fSpriteSize * 0.5f));
				shot->alive = true;
				shot->X = InvaderList[i].position.X;
				shot->Y = InvaderList[i].position.Y;
				ShotList.push_back(*shot);
			}
		}
	}
}

void Invader::ShotMovement(int iShotNumber) {
	Shot shot = ShotList[iShotNumber];
	//Catches any lasers who have lost their sprites
	shot.iShotSprite = UG::CreateSprite("../images/laser.png", (float)(4), (float)(16));
	//Only needs to check whether a barrier has been hit to destroy it as when the player is hit, the whole scene is reset
	if (!barriers.DetectHit(shot.X, shot.Y)) {
		if (shot.Y > ((768 / 8) - 16)) {
			//if the shot is still above the bottom line, it will continue to travel downwards
			shot.Y -= 6;
			UG::SetSpritePosition(shot.iShotSprite, shot.X, shot.Y);
			ShotList[iShotNumber] = shot;
			//detects whether it has hit the player or not
			playerPos.DetectHit(shot.X, shot.Y);
		}
		else {
			//if the Shot is below the bottom line, it is destroyed and removed from the vector
			//the shot count is decremented so that a new shot has the chance to be fired
			shot.alive = false;
			UG::DestroySprite(shot.iShotSprite);
			ShotList[iShotNumber] = shot;
			ShotList.erase(ShotList.begin() + iShotNumber);
			iShotCount--;
		}
	}
	else {
		//if the shot hits a barricade, it is destroyed and removed from the vector
		//the shot count is decremented so that a new shot has the chance to be fired
		shot.alive = false;
		UG::DestroySprite(shot.iShotSprite);
		ShotList[iShotNumber] = shot;
		ShotList.erase(ShotList.begin() + iShotNumber);
		iShotCount--;
	}
}

int Invader::GetLineCount() {
	return lineCount;
}
