#include "Game.h"
#include <windows.h>

#include "../Core/Utils.h"
#include "../Core/Renderer/ASCIIRenderer.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 200;

const int SCREEN_BOUNDARY_TOP = 40;
const int SCREEN_BOUNDARY_BOTTOM = SCREEN_HEIGHT - 5;
const int SCREEN_BOUNDARY_LEFT = 0;
const int SCREEN_BOUNDARY_RIGHT = 100;

const int MAX_PLAYER_PROJECTILES = 10;
const int MAX_ENEMY_PROJECTILES = 100;
const int MAX_EXPLOSION_SPRITES = 50;

#define VK_LEFT		0x25
#define VK_RIGHT	0x27
#define VK_SPACE	0x20       

bool Game::m_Keys[7] = { false };

enum MainMenuOptions
{
	MAIN_MENU_START = 1,
	MAIN_MENU_HIGHSCORE,
	MAIN_MENU_SCREENSIZE,
	MAIN_MENU_QUIT
};

enum PauseMenuOptions
{
	PAUSE_MENU_RESUME,
	PAUSE_MENU_SCREENSIZE,
	PAUSE_MENU_QUIT
};

Game::Game() :
	m_pRenderer(NULL),
	m_bInitialised(false),
	m_bExitApp(false),
	clouds(SCREEN_WIDTH, SCREEN_HEIGHT),
	m_GameState(E_GAME_STATE_MAIN_MENU)
{
	playerProjectiles.reserve(MAX_PLAYER_PROJECTILES);
	enemyProjectiles.reserve(MAX_ENEMY_PROJECTILES);
	m_Explosions.reserve(MAX_EXPLOSION_SPRITES);

	for (int i = 0; i < MAX_PLAYER_PROJECTILES; i++)
		playerProjectiles.emplace_back(PROJECTILE_PLAYER);

	for (int i = 0; i < MAX_ENEMY_PROJECTILES; i++)
		enemyProjectiles.emplace_back(PROJECTILE_ENEMY);

	for (int i = 0; i < MAX_EXPLOSION_SPRITES; i++)
	{
		m_Explosions.emplace_back();
		m_Explosions[i].SetSpriteSheet("../sprites/explosion.tga", 6);
	}

	scoreDisplay.SetPosition(10, 10);

	mainMenu.AddMenuItem(TGAFile("menu/menu_title.tga"));
	mainMenu.AddMenuItem(TGAFile("menu/menu_startgame.tga"));
	mainMenu.AddMenuItem(TGAFile("menu/menu_highscore.tga"));
	mainMenu.AddMenuItem(TGAFile("menu/menu_screensize.tga"));
	mainMenu.AddMenuItem(TGAFile("menu/menu_quit.tga"));
	mainMenu.SetPosition(150, 20);
	mainMenu.SetItemSpacing(5);
	mainMenu.SetSelectionIndex(1);
	mainMenu.SetMinimumIndex(1);
	mainMenu.AlignVertical();

	pauseMenu.AddMenuItem(TGAFile("menu/menu_resume.tga"));
	pauseMenu.AddMenuItem(TGAFile("menu/menu_screensize.tga"));
	pauseMenu.AddMenuItem(TGAFile("menu/menu_quit.tga"));
	pauseMenu.SetPosition(150, 35);
	pauseMenu.SetItemSpacing(5);
	pauseMenu.AlignVertical();
}

Game::~Game()
{
	SAFE_DELETE_PTR(m_pRenderer);
}

void Game::Initialise()
{
	InitialiseRenderer();

	enemies.push_back(Enemy(ENEMY_BIPLANE));
	enemies.push_back(Enemy(ENEMY_SPITFIRE));
	enemies.push_back(Enemy(ENEMY_GUNSHIP));

	player.SetPosition(0, SCREEN_HEIGHT / 2 - player.GetSize().y / 2);
	
	enemies[0].SetPosition(SCREEN_WIDTH, 40);
	enemies[1].SetPosition(SCREEN_WIDTH, 60);
	enemies[2].SetPosition(SCREEN_WIDTH, 80);

	scoreDisplay.Initialise();
	scoreDisplay.SetText("0");
	scoreDisplay.SetPosition(10, 5);

	m_bInitialised = true;
}

void Game::InitialiseRenderer()
{
	m_pRenderer = new ASCIIRenderer();

	m_pRenderer->Initialise(SCREEN_WIDTH, SCREEN_HEIGHT);
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

void Game::UpdateMainMenu()
{
	if (OnKeyPressed(VK_ESCAPE))
		m_bExitApp = true;

	else if (OnKeyPressed(VK_RETURN))
	{
		switch (mainMenu.GetSelectedMenuOption())
		{
		case MAIN_MENU_START:				// Start game
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
		pauseMenu.SetSelectionIndex(0);
		return;
	}

	UpdatePlayer(deltaTime);
	UpdateEnemies(deltaTime);

	UpdatePlayerProjectiles(deltaTime);
	UpdateEnemyProjectiles(deltaTime);

	for (SpriteAnimation& explosion : m_Explosions)
		if (explosion.Active())
			explosion.Update(deltaTime);

	static int lastScore = 0;

	if (player.GetScore() > lastScore)
		scoreDisplay.SetText(std::to_string(player.GetScore()));

	lastScore = player.GetScore();
}

void Game::RenderGame()
{
	scoreDisplay.Render(m_pRenderer);

		//render your game here
	player.Render(m_pRenderer);

	for (int i = 0; i < enemies.size(); i++)
		if (!enemies[i].IsDestroyed())
			enemies[i].Render(m_pRenderer);

	RenderProjectiles();

	RenderExplosions();
}

void Game::RenderExplosions()
{
	for (SpriteAnimation& explosion : m_Explosions)
		if (explosion.Active())
			explosion.Render(m_pRenderer);
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
				continue;
			}

			if (enemies[i].Fire())
			{
				Projectile& enemyProjectile = GetEnemyProjectile();
				enemyProjectile.SetFiringState(true);
				enemyProjectile.SetPosition(enemies[i].GetPosition().x, enemies[i].GetPosition().y + (enemies[i].GetSize().y / 2));
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
							SetExplosion(enemies[j]);
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

void Game::RenderProjectiles()
{
	for (int i = 0; i < playerProjectiles.size(); i++)
		if (playerProjectiles[i].IsFiring())
			playerProjectiles[i].Render(m_pRenderer);

	for (int i = 0; i < enemyProjectiles.size(); i++)
		if (enemyProjectiles[i].IsFiring())
			enemyProjectiles[i].Render(m_pRenderer);
}

Projectile& Game::GetProjectile(std::vector<Projectile>& array, ProjectileType type)
{
	// Return the first inactive projectile in the vector
	for (Projectile& projectile : array)
		if (!projectile.IsFiring())
			return projectile;

	// All projectiles are active, push back and return a new projectile
	array.emplace_back(type);
	return array.back();
}

Projectile& Game::GetPlayerProjectile()
{
	return GetProjectile(playerProjectiles, PROJECTILE_PLAYER);
}

Projectile& Game::GetEnemyProjectile()
{
	return GetProjectile(enemyProjectiles, PROJECTILE_ENEMY);
}

void Game::SetExplosion(Plane& plane)
{
	for (SpriteAnimation& explosion : m_Explosions)
	{
		if (!explosion.Active())
		{
			explosion.SetPosition((plane.GetPosition().x + (plane.GetSize().x / 2)) - (explosion.GetSize().x / 2), 
								  (plane.GetPosition().y + (plane.GetSize().y / 2)) - (explosion.GetSize().y / 2));
			explosion.SetFrame(0);
			explosion.SetActive(true);
			break;
		}
	}
}