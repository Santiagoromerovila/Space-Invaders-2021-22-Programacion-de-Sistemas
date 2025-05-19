#ifndef Invader_H
#define Invader_H

class Invader
{
public:
	Invader();
	~Invader();
	bool DefeatState = false;
	int iInvaderScore;
	int GetLineCount();
	void Initialise();
	void DrawInvaders();
	void Movement();
	bool DetectHit(float X, float Y);
	void Shoot();
	void ShotMovement(int iShotNumber);

private:
	int shootFrameCount;
	int frameCount;
	int lineCount = 0;
	int direction = 1;
	int iShotCount = 0;
	float speed;
};

#endif
