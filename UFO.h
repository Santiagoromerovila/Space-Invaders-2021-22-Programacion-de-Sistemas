#ifndef UFO_H
#define UFO_H

class UFO
{
public:
	UFO();
	~UFO();

	int iUFOScore;
	bool onScreen = false;
	void Spawn();
	void Movement();
	bool HitDetection(float shotX, float shotY);
	void Initialise();
	int Score();

private:
	int frameCount = 0;
};

#endif