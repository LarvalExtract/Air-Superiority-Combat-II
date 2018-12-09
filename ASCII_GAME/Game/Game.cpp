#include "Game.h"
#include <windows.h>

#include "../Core/Utils.h"
#include "../Core/Renderer/ASCIIRenderer.h"

constexpr int HALF_SCREEN_WIDTH = SCREEN_WIDTH / 2;
constexpr int HALF_SCREEN_HEIGHT = SCREEN_HEIGHT / 2;

constexpr int SCREEN_BOUNDARY_TOP = 40;
constexpr int SCREEN_BOUNDARY_BOTTOM = SCREEN_HEIGHT - 5;
constexpr int SCREEN_BOUNDARY_LEFT = 0;
constexpr int SCREEN_BOUNDARY_RIGHT = 100;

constexpr int MAX_PLAYER_LIVES = 4;
constexpr int MAX_ENEMIES = 3;
constexpr int MAX_PLAYER_PROJECTILES = 10;
constexpr int MAX_ENEMY_PROJECTILES = 100;
constexpr int MAX_EXPLOSION_SPRITES = 50;

#define VK_LEFT		0x25
#define VK_RIGHT	0x27
#define VK_SPACE	0x20       

bool Game::m_Keys[7] = { false };

Texture playerHealthIcon(TGAFile("health_icon.tga"));
TextureArray explosion(TGAFile("explosion.tga"), 6);

enum E_MAIN_MENU_OPTIONS : unsigned char
{
	MAIN_MENU_START = 1,
	MAIN_MENU_HIGHSCORE,
	MAIN_MENU_SCREENSIZE,
	MAIN_MENU_QUIT
};

enum E_PAUSE_MENU_OPTIONS : unsigned char
{
	PAUSE_MENU_RESUME = 1,
	PAUSE_MENU_SCREENSIZE,
	PAUSE_MENU_QUIT
};

Game::Game() :
	m_pRenderer(NULL),
	m_bInitialised(false),
	m_bExitApp(false),
	clouds(SCREEN_WIDTH, SCREEN_HEIGHT),
	m_GameState(E_GAME_STATE_MAIN_MENU),
	enemies(MAX_ENEMIES),
	playerProjectiles(MAX_PLAYER_PROJECTILES, Projectile::s_playerProjectile),
	enemyProjectiles(MAX_ENEMY_PROJECTILES)
{
	
}

Game::~Game()
{
	SAFE_DELETE_PTR(m_pRenderer);
}

void Game::Initialise()
{
	if (m_bInitialised)
		return;

	m_pRenderer = new ASCIIRenderer();
	m_pRenderer->Initialise(SCREEN_WIDTH, SCREEN_HEIGHT);

	scoreDisplay.Initialise();
	scoreDisplay.SetText("0");
	scoreDisplay.SetPosition(10, 5);

	mainMenu.AddMenuItem(TGAFile("menu/menu_title.tga"));
	mainMenu.AddMenuItem(TGAFile("menu/menu_startgame.tga"));
	mainMenu.AddMenuItem(TGAFile("menu/menu_highscore.tga"));
	mainMenu.AddMenuItem(TGAFile("menu/menu_screensize.tga"));
	mainMenu.AddMenuItem(TGAFile("menu/menu_quit.tga"));
	mainMenu.SetPosition(150, 20);
	mainMenu.SetItemSpacing(5);
	mainMenu.SetMinimumIndex(MAIN_MENU_START);
	mainMenu.AlignVertical();

	pauseMenu.AddMenuItem(TGAFile("menu/menu_paused.tga"));
	pauseMenu.AddMenuItem(TGAFile("menu/menu_resume.tga"));
	pauseMenu.AddMenuItem(TGAFile("menu/menu_screensize.tga"));
	pauseMenu.AddMenuItem(TGAFile("menu/menu_quit.tga"));
	pauseMenu.SetPosition(150, 30);
	pauseMenu.SetItemSpacing(5);
	pauseMenu.SetMinimumIndex(PAUSE_MENU_RESUME);
	pauseMenu.AlignVertical();

	player.SetMaxLives(MAX_PLAYER_LIVES);
	m_PlayerLifeIcons = new Sprite[MAX_PLAYER_LIVES];

	// Position health icons at top middle of screen
	for (char i = 0; i < MAX_PLAYER_LIVES; i++)
	{
		m_PlayerLifeIcons[i].SetTexture(playerHealthIcon);
		m_PlayerLifeIcons[i].SetPosition(HALF_SCREEN_WIDTH - ((MAX_PLAYER_LIVES * m_PlayerLifeIcons[i].GetSize().x + (MAX_PLAYER_LIVES - 1) * 7) / 2) + (i * m_PlayerLifeIcons[i].GetSize().x) + (i * 7), 6);
	}

	explosionSprite.SetTextureArray(explosion);
	explosionSprite.SetFrameTime(0.035f);

	m_bInitialised = true;
}

void Game::Run()
{
	float lastTime = 0.0f;
	float newTime = gameTimer.Elapsed();
	float deltaTime;

	while (!m_bExitApp)
	{
		newTime = gameTimer.Elapsed();
		deltaTime = newTime - lastTime;
		lastTime = newTime;

		Update(deltaTime);

		Render();
	}
}

void Game::InitialiseGame()
{
	player.SetPosition(0, HALF_SCREEN_HEIGHT - player.GetSize().y / 2);
	player.SetLives(MAX_PLAYER_LIVES);
	player.SetScore(0);
	
	scoreDisplay.SetText(std::to_string(player.GetScore()));

	for (Projectile& proj : playerProjectiles)
		proj.SetFiringState(false);

	for (Projectile& proj : enemyProjectiles)
		proj.SetFiringState(false);

	for (Enemy& enemy : enemies)
	{
		enemy.ResetHealth();
		enemy.SetActive(false);
	}

	roundTimer.Reset();
}

void Game::Update(float deltaTime)
{
	if (m_GameState != E_GAME_STATE_PAUSED)
		clouds.Update(deltaTime);

	switch (m_GameState)
	{
	case E_GAME_STATE_MAIN_MENU:	UpdateMainMenu(); break;
	case E_GAME_STATE_IN_GAME:		UpdateGame(deltaTime); break;
	case E_GAME_STATE_PAUSED:		UpdatePauseMenu(); break;
	}
}

void Game::Render()
{
	//call this first
	m_pRenderer->ClearScreen(ConsoleColour::BACKGROUND_SKYBLUE);

	// Render background
	clouds.Render(m_pRenderer);

	switch (m_GameState)
	{
	case E_GAME_STATE_MAIN_MENU:	mainMenu.Render(m_pRenderer); break;
	case E_GAME_STATE_IN_GAME:		RenderGame(); break;
	case E_GAME_STATE_PAUSED:		RenderGame(); pauseMenu.Render(m_pRenderer); break;
	}

	//call this last
	m_pRenderer->Render();
}

void Game::UpdateMainMenu()
{
	if (OnKeyPressed(VK_ESCAPE))
		m_bExitApp = true;

	else if (OnKeyPressed(VK_RETURN))
	{
		switch (mainMenu.GetSelectedMenuOption())
		{
		case MAIN_MENU_START:				// Start game
			InitialiseGame();
			m_GameState = E_GAME_STATE_IN_GAME;
			break;
		case MAIN_MENU_HIGHSCORE:			// Show highest score

			break;
		case MAIN_MENU_SCREENSIZE:			// Toggle screen size
			m_pRenderer->TogglePixelSize();
			break;
		case MAIN_MENU_QUIT:				// Close game
			m_bExitApp = true;
			break;
		}
	}

	else if (OnKeyPressed(VK_UP))
		mainMenu.DecrementMenuIndex();

	else if (OnKeyPressed(VK_DOWN))
		mainMenu.IncrementMenuIndex();
}

void Game::UpdatePauseMenu()
{
	if (OnKeyPressed(VK_ESCAPE))
		m_GameState = E_GAME_STATE_IN_GAME;

	else if (OnKeyPressed(VK_RETURN))
	{
		switch (pauseMenu.GetSelectedMenuOption())
		{
		case PAUSE_MENU_RESUME:
			m_GameState = E_GAME_STATE_IN_GAME;
			break;
		case PAUSE_MENU_SCREENSIZE: 
			m_pRenderer->TogglePixelSize(); 
			break;
		case PAUSE_MENU_QUIT:		
			m_bExitApp = true; 
			break;
		}
	}

	else if (OnKeyPressed(VK_UP))
		pauseMenu.DecrementMenuIndex();

	else if (OnKeyPressed(VK_DOWN))
		pauseMenu.IncrementMenuIndex();
}

void Game::UpdateGame(float deltaTime)
{
	if (OnKeyPressed(VK_ESCAPE))
	{
		m_GameState = E_GAME_STATE_PAUSED;
		pauseMenu.SetSelectionIndex(PAUSE_MENU_RESUME);
		return;
	}

	UpdatePlayer(deltaTime);
	UpdateEnemies(deltaTime);

	UpdatePlayerProjectiles(deltaTime);
	UpdateEnemyProjectiles(deltaTime);

	for (auto it = m_Explosions.begin(); it != m_Explosions.end(); )
	{
		it->Update(deltaTime);

		// Remove explosion after it's played once
		it->GetLoopCount() > 0 ? it = m_Explosions.erase(it) : it++;
	}

	static int lastScore = 0;

	if (player.GetScore() > lastScore)
		scoreDisplay.SetText(std::to_string(player.GetScore()));

	lastScore = player.GetScore();
}

void Game::UpdatePlayer(float deltaTime)
{
	player.Update(deltaTime);

	// Prevent player from going above the screen
	if (player.GetPosition().y < SCREEN_BOUNDARY_TOP)
		player.SetPosition(player.GetPosition().x, SCREEN_BOUNDARY_TOP);

	// Prevent player from going below screen
	else if (player.GetPosition().y + player.GetSize().y > SCREEN_HEIGHT)
		player.SetPosition(player.GetPosition().x, SCREEN_HEIGHT - player.GetSize().y);

	// Prevent player from going left of screen
	if (player.GetPosition().x < 0)
		player.SetPosition(0, player.GetPosition().y);

	// Prevent player from going too far right
	else if (player.GetPosition().x > SCREEN_BOUNDARY_RIGHT)
		player.SetPosition(SCREEN_BOUNDARY_RIGHT, player.GetPosition().y);

	if (player.Fire())
	{
		Projectile& proj = GetPlayerProjectile();
		proj.SetPosition(player.GetPosition().x + player.GetSize().x, (player.GetPosition().y + (player.GetSize().y / 2)) - proj.GetSize().y / 2);
		proj.SetVelocity(Vec2<float>(400.0f, 0.0f));
		proj.SetFiringState(true);
	}
}

void Game::UpdateEnemies(float deltaTime)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		// Enemy is alive on screen
		if (!enemies[i].IsDestroyed())
		{
			enemies[i].Update(deltaTime);

			// Check if enemy has escaped screen
			if (enemies[i].GetPosition().x + enemies[i].GetSize().x < 0)
			{
				enemies[i].SetActive(false);
				player.DecrementLives();
				continue;
			}

			if (enemies[i].Fire())
			{
				enemies[i].SetProjectile(GetEnemyProjectile());
			}
		}
		// Enemy is ready to be spawned
		else
		{
			if (Random(1, 100) < 3)
			{
				enemies[i].SetPosition(SCREEN_WIDTH, Random(SCREEN_BOUNDARY_TOP, SCREEN_HEIGHT - enemies[i].GetSize().y));
				enemies[i].ResetHealth();
				enemies[i].SetActive();
			}
		}
	}
}

void Game::UpdatePlayerProjectiles(float deltaTime)
{
	for (Projectile& projectile : playerProjectiles)
	{
		if (projectile.IsFiring())
		{
			projectile.Update(deltaTime);

			// Disable projectile if it's left the screen, and skip to the next projectile
			if (projectile.GetPosition().x > SCREEN_WIDTH)
			{
				projectile.SetFiringState(false);
				continue;
			}

			// Check if the projectile has hit any of the enemies on screen
			for (int j = 0; j < enemies.size(); j++)
			{
				if (!enemies[j].IsDestroyed())
				{
					if (projectile.Collides(enemies[j]))
					{
						enemies[j].ApplyDamage(projectile.GetDamage());

						if (enemies[j].IsDestroyed())
						{
							player.AddScore(enemies[j].GetPoints());
							m_Explosions.push_back(explosionSprite);

							m_Explosions.back().SetPosition(
								enemies[j].GetPosition().x,
								enemies[j].GetPosition().y);
						}

						projectile.SetFiringState(false);

						break;
					}
				}
			}
		}
	}
}

void Game::UpdateEnemyProjectiles(float deltaTime)
{
	for (Projectile& projectile : enemyProjectiles)
	{
		if (projectile.IsFiring())
		{
			projectile.Update(deltaTime);

			// Disable current projectile if it has gone off screen, and skip to the next projectile
			if (projectile.GetPosition().x + projectile.GetSize().x < 0)
			{
				projectile.SetFiringState(false);
				continue;
			}

			if (projectile.Collides(player))
			{
				player.ApplyDamage(1);

				projectile.SetFiringState(false);
			}
		}
	}
}

void Game::RenderGame()
{
	scoreDisplay.Render(m_pRenderer);

	for (char i = 0; i < player.GetLives(); i++)
		m_PlayerLifeIcons[i].Render(m_pRenderer);

	player.Render(m_pRenderer);

	for (int i = 0; i < enemies.size(); i++)
		if (!enemies[i].IsDestroyed())
			enemies[i].Render(m_pRenderer);

	RenderProjectiles();

	RenderExplosions();
}

void Game::RenderProjectiles()
{
	for (int i = 0; i < playerProjectiles.size(); i++)
		if (playerProjectiles[i].IsFiring())
			playerProjectiles[i].Render(m_pRenderer);

	for (int i = 0; i < enemyProjectiles.size(); i++)
		if (enemyProjectiles[i].IsFiring())
			enemyProjectiles[i].Render(m_pRenderer);
}

void Game::RenderExplosions()
{
	for (auto it = m_Explosions.begin(); it != m_Explosions.end(); it++)
		it->Render(m_pRenderer);
}

Projectile& Game::GetPlayerProjectile()
{
	for (Projectile& projectile : playerProjectiles)
		if (!projectile.IsFiring())
			return projectile;

	playerProjectiles.push_back(Projectile());
	return playerProjectiles.back();
}

Projectile& Game::GetEnemyProjectile()
{
	for (Projectile& projectile : enemyProjectiles)
		if (!projectile.IsFiring())
			return projectile;

	enemyProjectiles.push_back(Projectile());
	return enemyProjectiles.back();
}

bool Game::OnKeyPressed(int keycode)
{
	int index = 0;

	switch (keycode)
	{
	case VK_UP:		index = KEY_UP;		break;
	case VK_LEFT:	index = KEY_LEFT;	break;
	case VK_RIGHT:	index = KEY_RIGHT;	break;
	case VK_DOWN:	index = KEY_DOWN;	break;
	case VK_SPACE:	index = KEY_SPACE;	break;
	case VK_ESCAPE:	index = KEY_ESCAPE;	break;
	case VK_RETURN:	index = KEY_RETURN;	break;
	}

	if (GetKeyState(keycode) < 0)
	{
		if (!m_Keys[index])
		{
			m_Keys[index] = true;
			return true;
		}

		return false;
	}

	m_Keys[index] = false;
	return false;
}