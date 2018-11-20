#ifndef _GAME_H_
#define _GAME_H

#include "GameStates.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../Projectile.h"
#include "../Background.h"
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
	void UpdatePlayerProjectiles();
	void UpdateEnemyProjectiles();

	void RenderProjectiles();

	//Variables
	ASCIIRenderer* m_pRenderer;
	Background clouds;

	std::vector<Projectile> playerProjectiles;
	std::vector<Projectile> enemyProjectiles;

	Projectile& GetPlayerProjectile();
	Projectile& GetEnemyProjectile();
	Projectile& GetProjectile(std::vector<Projectile>& array, ProjectileType type);

	Player player;
	std::vector<Enemy*> pEnemies;

	bool m_bInitialised;
	bool m_bExitApp;

	int m_Checkpoint;

	Timer<float> gameTimer;
	float m_deltaTime;
};


#endif
