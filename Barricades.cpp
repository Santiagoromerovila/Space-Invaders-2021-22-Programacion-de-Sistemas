#include "Barricades.h"
#include "UGFW.h"
#include "UG_Defines.h"
#include <iostream>
#include <vector>


Barricades::Barricades()
{
}


Barricades::~Barricades()
{
}

struct BarrPortion {
	//Each Barricade portion has an X/Y, Sprite ID and health (each portion can be hit twice before being destroyed)
	float X;
	float Y;
	int iHealth = 2;
	int iBarricadeSprite = 1;
};

//Vector to contain all the portions (or 'Bricks') of the barricades
std::vector<BarrPortion> Bricks;

void Barricades::Initialise() {
	//makes sure the vector is empty then reserves an appropriate size to hold all the bricks
	Bricks.clear();
	Bricks.reserve(40);
	//X/Y of the starting brick (incremented on for each new brick)
	float iStartX = 64;
	float iStartY = 200;
	// 4 times - one for each barricade
	for (int i = 0; i < 4; i++) {
		//The structure of a barricade is 3223 and so this is reflected upon in these loops
		for (int j = 0; j < 3; j++) {
			//creates 3 bricks vertically from the start point
			CreateBrick(iStartX, iStartY, j);
		}
		//moves one the the right and one up to start the two 2's
		iStartX += SpriteSize;
		iStartY += SpriteSize;
		for (int k = 0; k < 2; k++) {
			for (int l = 0; l < 2; l++) {
				//creates 2 vertically from the start point (which has been incremented to be 1 right and 1 up from origin)
				CreateBrick(iStartX, iStartY, l);
			}
			//this is looped twice for the two 2's so the start X must be moved along one
			iStartX += SpriteSize;
		}
		//last row of 3, moved back down one to be on the same level as the first 3
		iStartY -= SpriteSize;
		for (int m = 0; m < 3; m++) {
			//creates 3 vertically from the new start point
			CreateBrick(iStartX, iStartY, m);
		}
		//moves the start horizontally by 138 for the next barricade to be created at
		iStartX += 142;
	}
}

void Barricades::CreateBrick(float X, float Y, int i) {
	//instantiates a brick with a Sprite and X/Y, i from the loop is passed in so it knows what brick it is on and so can move the next up by the sprite size * i
	BarrPortion* brick = new BarrPortion();
	brick->iBarricadeSprite = UG::CreateSprite("../images/barrier.png", (float)(SpriteSize), (float)(SpriteSize), true);
	brick->X = (X);
	brick->Y = (Y + (SpriteSize * i));
	UG::SetSpritePosition(brick->iBarricadeSprite, brick->X, brick->Y);
	//Adds the new brick to the vector
	Bricks.push_back(*brick);
}

bool Barricades::DetectHit(float X, float Y) {
	//Called by both the player's laser and the invaders laser as both can break bricks
	for (int i = 0; i < Bricks.size(); i++) {
		BarrPortion curBrick = Bricks[i];
		//every brick checks if it shares an X/Y with a laser (a hit)
		if (X > (curBrick.X - SpriteSize * 0.5) && X < (curBrick.X + SpriteSize * 0.5)) {
			if (Y > (curBrick.Y - SpriteSize * 0.5) && Y < (curBrick.Y + SpriteSize * 0.5)) {
				//hit bricks health is decremented, if it reaches 0, it is destroyed and removed from the vector
				Bricks[i].iHealth--;
				if (Bricks[i].iHealth == 0) {
					UG::DestroySprite(curBrick.iBarricadeSprite);
					Bricks.erase(Bricks.begin() + i);
				}
				//returns true to the lasers so that they know theyve hit something and can destroy themselves
				return true;
				break;
			}
		}
	}
	//if no barrier is hit then the lasers are free to continue on
	return false;
}

void Barricades::DrawBarricades() {
	//draws each brick in the vector
	for (int i = 0; i < Bricks.size(); i++) {
		UG::DrawSprite(Bricks[i].iBarricadeSprite);
	}
}