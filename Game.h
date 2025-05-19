#ifndef Game_H
#define Game_H

enum Game_State
{
	MENU,
	PLAY,
	PAUSE,
	DEFEAT,
	EXIT,
};

class Game
{
public:
	Game();
	~Game();
	bool gameStarted = false;
	bool Initialise();
	bool Update();
	void Draw();
	void Deinitialise();
	void Reset();
	void DrawScore();
	Game_State  m_eCurrentGameState;
	void ScoreHolder(int points);


private:
	int menu;
	int pauseScreen;
	int defeatScreen;
	bool PlayerAlive;
};

#endif // Game_H
