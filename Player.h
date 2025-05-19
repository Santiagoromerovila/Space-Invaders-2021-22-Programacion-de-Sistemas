#ifndef Player_H
#define Player_H

class Player
{
public:
	Player();
	~Player();
	int GetLives();
	void SetLives();
	bool Alive;
	void Movement();
	void Initialise();
	void DrawPlayer();
	void shootLaser();
	void DetectHit(float X, float Y);
	void DrawLives();
	int shotCount = 0;
};

#endif // Player_H