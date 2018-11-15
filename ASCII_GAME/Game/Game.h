#ifndef _GAME_H_
#define _GAME_H

#include "GameStates.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../Projectile.h"
#include <vector>
#include <list>

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
	void UpdatePlayerProjectiles();
	void UpdateEnemyProjectiles();

	void RenderProjectiles();

	//Variables
	ASCIIRenderer* m_pRenderer;

	std::vector<Projectile> playerProjectiles;
	std::vector<Projectile> enemyProjectiles;

	Projectile& GetPlayerProjectile();
	Projectile& GetEnemyProjectile();

	Player player;
	std::vector<Enemy*> pEnemies;

	bool m_bInitialised;
	bool m_bExitApp;

	int m_Checkpoint;
};


#endif
