#ifndef _GAME_H_
#define _GAME_H

#include "GameStates.h"
#include "GameObjects/Player.h"
#include "GameObjects/Enemy.h"
#include "GameObjects/Projectile.h"
#include "GameObjects/Background.h"
#include "GameObjects/Menu.h"
#include "Core/Renderer/SpriteText.h"
#include "Core/Renderer/SpriteAnimation.h"
#include "Core/Timer.h"

#include <vector>

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

constexpr int SCREEN_WIDTH = 600;
constexpr int SCREEN_HEIGHT = 200;

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
	void InitialiseGame();
	void Update(float deltaTime);
	void Render();

	// Update functions

	// Handle player input in main menu
	void UpdateMainMenu();

	// Handle player input in pause menu
	void UpdatePauseMenu();

	// Update game's state
	void UpdateGame(float deltaTime);

	// Update player's state
	void UpdatePlayer(float deltaTime);

	// Update state of all enemies on screen
	void UpdateEnemies(float deltaTime);

	// Update state of all player projectiles on screen
	void UpdatePlayerProjectiles(float deltaTime);

	// Update state of all enemy projectiles on screen
	void UpdateEnemyProjectiles(float deltaTime);

	// Render functions
	void RenderGame();
	void RenderProjectiles();
	void RenderExplosions();

	bool OnKeyPressed(int keycode);

	//Variables
	ASCIIRenderer* m_pRenderer;
	Background clouds;

	std::vector<Projectile> playerProjectiles;
	std::vector<Projectile> enemyProjectiles;
	std::vector<SpriteAnimation> m_Explosions;

	// Returns the first inactive player projectile
	Projectile& GetPlayerProjectile();

	// Returns the first inactive enemy projectile
	Projectile& GetEnemyProjectile();

	Player player;
	std::vector<Enemy> enemies;

	Timer<float> gameTimer;
	Timer<float> roundTimer;

	E_GAME_STATE m_GameState;

	Menu mainMenu;
	Menu pauseMenu;

	SpriteText scoreDisplay;
	Sprite* m_PlayerLifeIcons;
	SpriteAnimation explosionSprite;

	static bool m_Keys[7];
};


#endif
