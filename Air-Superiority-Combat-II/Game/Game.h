#ifndef _GAME_H_
#define _GAME_H

#include "GameStates.h"
#include "GameObjects/Player.h"
#include "GameObjects/Enemy.h"
#include "GameObjects/Projectile.h"
#include "GameObjects/Background.h"
#include "GameObjects/Menu.h"
#include "Game/GameObjects/DigitDisplay.h"
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
	bool m_bPlayerDied;
	bool m_bEndlessMode;

	//Functions
	void InitialiseGame();
	void Update(float deltaTime);
	void Render();

	void InitialiseGameOverScreen();
	void InitialiseYouWinScreen();

	// Update functions

	// Handle player input in main menu
	void UpdateMainMenu();

	// Handle player input in pause menu
	void UpdatePauseMenu();

	// Update game's state
	void UpdateGame(float deltaTime);

	// Update game over screen
	void UpdateGameOverScreen(float deltaTime);

	void UpdateYouWinScreen(float deltaTime);

	// Update player's state
	void UpdatePlayer(float deltaTime);

	// Update state of all enemies on screen
	void UpdateEnemies(float deltaTime);

	// Update state of all player projectiles on screen
	void UpdatePlayerProjectiles(float deltaTime);

	// Update state of all enemy projectiles on screen. Returns true if player was destroyed
	void UpdateEnemyProjectiles(float deltaTime);

	void IncreaseDifficulty();

	// Render functions
	void RenderGame();
	void RenderGameOverScreen();
	void RenderYouWinScreen();

	void ResetGameObjects();
	void ResetEnemies();
	void ResetAllProjectiles();
	bool OnKeyPressed(int keycode);
	void DrawExplosion(Vec2<float> &position);
	void UpdateExplosionFrames(float deltaTime);
	void RespawnPlayer(float respawnTime);

	//Variables
	ASCIIRenderer* m_pRenderer;
	Background clouds;

	std::vector<Projectile> playerProjectiles;
	std::vector<Projectile> enemyProjectiles;

	std::vector<SpriteAnimation> m_Explosions;

	// Returns the first inactive projectile from vector of projectiles
	Projectile& GetProjectile(std::vector<Projectile> &projectiles);

	Player player;
	std::vector<Enemy> enemies;

	Timer<float> gameTimer;
	Timer<float> roundTimer;
	float m_deltaTime;

	E_GAME_STATE m_GameState;

	Menu mainMenu;
	Menu pauseMenu;

	Sprite splash;
	Sprite youWin;
	Sprite gameOver;
	Sprite highScore;
	Sprite pressEsc;

	DigitDisplay scoreDisplay;
	SpriteAnimation explosionSprite;
	std::vector<Sprite> m_PlayerLifeIcons;

	static bool m_Keys[7];

	unsigned char enemySpawnChance;
	unsigned char m_KillCount;
};


#endif
