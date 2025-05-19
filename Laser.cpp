#include "Laser.h"
#include "Player.h"
#include "UGFW.h"
#include "UG_Defines.h"
#include <iostream>
#include "Game.h"
#include "Invader.h"
#include "UFO.h"
#include <time.h>
#include "Barricades.h"

Laser::Laser()
{
	//laser starts off as dead until the player calls it
	isAlive = false;
}

Laser::~Laser()
{
}

struct laser {
	//laser has a sprite ID and X/Y
	int iLaserSprite;
	float X = 0;
	float Y = 0;
};

laser laserShot;
Invader invaderDet;
UFO ufo;
Barricades cover;

void Laser::Shoot(float X, float Y) {
	//Only one shot can be active at once
	if (!isAlive) {
		//initialised, placed 30 above the player
		laserShot.iLaserSprite = UG::CreateSprite("../images/laser.png", (float)(4), (float)(16));
		UG::SetSpritePosition(laserShot.iLaserSprite, X, Y + 30);
		laserShot.X = X;
		laserShot.Y = Y + 30;
		//drawn and set alive (Movement is called from Player to keep in the same frames)
		UG::DrawSprite(laserShot.iLaserSprite);
		isAlive = true;
	}
}

void Laser::Movement() {
	if (isAlive) {
		if (laserShot.Y < 768) {
			//while the laser is still on screen, its Y position is continually incremented
			laserShot.Y += 12;
			UG::SetSpritePosition(laserShot.iLaserSprite, laserShot.X, laserShot.Y);
			UG::DrawSprite(laserShot.iLaserSprite);
			if (invaderDet.DetectHit(laserShot.X, laserShot.Y) || ufo.HitDetection(laserShot.X, laserShot.Y) || cover.DetectHit(laserShot.X, laserShot.Y))
			{
				//If the laser hits an invader, UFO or barricade, it is dead
				//the logic for dealing with being hit is called within their detection of being hit
				isAlive = false;
			}
		}
		else {
			//is destroyed if off screen
			UG::DestroySprite(laserShot.iLaserSprite);
			isAlive = false;
		}
	}
	else {
		//is destroyed if it has hit an object
		UG::DestroySprite(laserShot.iLaserSprite);
		isAlive = false;
	}
}

