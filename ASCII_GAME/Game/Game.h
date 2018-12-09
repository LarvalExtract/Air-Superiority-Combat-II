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
#include "GameObjects/ScoreDisplay.h"
#include <vector>

#include "../Timer.h"

class ASCIIRenderer;

enum E_KEYS
{
	KEY_UP,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_SPACE,
	KEY_ESCAPE,
	KEY_RETURN
};

class Game
{
public:
	Game();
	~Game();

	void Initialise();
	void Run();

private:
	bool m_bInitialised;
	bool m_bExitApp;

	//Functions
	void InitialiseRenderer();
	void Update(float deltaTime);
	void Render();

	bool OnKeyPressed(int keycode);

	// Update functions
	void UpdateMainMenu();
	void UpdatePauseMenu();
	void UpdateGame(float deltaTime);
	void UpdatePlayer(float deltaTime);
	void UpdateEnemies(float deltaTime);
	void UpdatePlayerProjectiles(float deltaTime);
	void UpdateEnemyProjectiles(float deltaTime);

	// Render functions
	void RenderGame();
	void RenderProjectiles();
	void RenderExplosions();

	void SetExplosion(Plane& plane);

	//Variables
	ASCIIRenderer* m_pRenderer;
	Background clouds;

	std::vector<Projectile> playerProjectiles;
	std::vector<Projectile> enemyProjectiles;
	std::vector<SpriteAnimation> m_Explosions;

	Projectile& GetPlayerProjectile();
	Projectile& GetEnemyProjectile();
	Projectile& GetProjectile(std::vector<Projectile>& array, ProjectileType type);

	Player player;
	std::vector<Enemy> enemies;

	Timer<float> gameTimer;

	E_GAME_STATE m_GameState;

	Menu mainMenu;
	Menu pauseMenu;

	SpriteText scoreDisplay;

	static bool m_Keys[7];
};


#endif
