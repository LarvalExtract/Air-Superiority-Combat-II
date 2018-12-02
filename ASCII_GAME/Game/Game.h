#ifndef _GAME_H_
#define _GAME_H

#include "GameStates.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../Projectile.h"
#include "../Background.h"
#include "../MainMenu.h"
#include "../SpriteText.h"
#include "../SpriteAnimation.h"
#include <vector>

#include "../Timer.h"

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
	void Update(float deltaTime);
	void Render();

	void UpdateMainMenu();
	void UpdateGame(float deltaTime);
	void UpdatePlayer(float deltaTime);
	void UpdatePlayerProjectiles(float deltaTime);
	void UpdateEnemyProjectiles(float deltaTime);

	void RenderGame();
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

	Timer<float> gameTimer;

	E_GAME_STATE m_GameState;

	MainMenu mainMenu;
	SpriteText m_ScoreDisplay;
	SpriteAnimation animTest;
};


#endif
