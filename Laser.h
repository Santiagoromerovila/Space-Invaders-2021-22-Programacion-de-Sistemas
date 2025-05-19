#ifndef Laser_H
#define Laser_H

class Laser
{
public:
	Laser();
	~Laser();
	void Shoot(float X, float Y);
	void Movement();
	bool isAlive = false;
};

#endif