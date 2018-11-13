#ifndef _GAME_H_
#define _GAME_H

#include "GameStates.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../ProjectileManager.h"
#include <vector>

class ASCIIRenderer;

class Game
{
public:
	Game();
	~Game();

	void Initialise();
	void Run();

private:

	//Functions
	void InitialiseRenderer();
	void Update();
	void Render();

	void ProcessInputs();

	//Variables
	ASCIIRenderer* m_pRenderer;

	ProjectileManager m_ScreenProjectiles;

	Player player;
	std::vector<Enemy*> pEnemies;

	bool m_bInitialised;
	bool m_bExitApp;

	int m_Checkpoint;
};


#endif
