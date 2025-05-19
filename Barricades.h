#ifndef Barricades_H
#define Barricades_H

class Barricades
{
public:
	Barricades();
	~Barricades();
	void Initialise();
	bool DetectHit(float X, float Y);
	void CreateBrick(float X, float Y, int i);
	void DrawBarricades();

private:
	float SpriteSize = 32;
};

#endif