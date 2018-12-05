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
}

Game::~Game()
{
	for (int i = 0; i < pEnemies.size(); i++)
		if (pEnemies[i] != nullptr)
			delete pEnemies[i];

	SAFE_DELETE_PTR(m_pRenderer);
}

void Game::Initialise()
{
	InitialiseRenderer();
	
	mainMenu.Initialise(SCREEN_WIDTH);

	pEnemies.push_back(new Enemy(ENEMY_BIPLANE));
	pEnemies.push_back(new Enemy(ENEMY_MEDIUM));
	pEnemies.push_back(new Enemy(ENEMY_GUNSHIP));

	player.SetPosition(0, SCREEN_HEIGHT / 2 - player.GetSize().y / 2);
	
	pEnemies[0]->SetPosition(SCREEN_WIDTH, 40);
	pEnemies[1]->SetPosition(SCREEN_WIDTH, 60);
	pEnemies[2]->SetPosition(SCREEN_WIDTH, 80);

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
	clouds.Update(deltaTime);

	switch (m_GameState)
	{
	case E_GAME_STATE_MAIN_MENU:	UpdateMainMenu(); break;
	case E_GAME_STATE_IN_GAME:		UpdateGame(deltaTime); break;
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
	}

	//call this last
	m_pRenderer->Render();
}

void Game::UpdateMainMenu()
{
	mainMenu.Update();

	static bool bIsReturnPressed = false;
	if (GetKeyState(VK_RETURN) < 0)
	{
		if (!bIsReturnPressed)
		{
			switch (mainMenu.GetSelectedMenuOption())
			{
			case OPTION_START_GAME:				// Start game
				m_GameState = E_GAME_STATE_IN_GAME;
				break;
			case OPTION_HIGH_SCORE:				// Show highest score

				break;
			case OPTION_CHANGE_SCREEN_SIZE:		// Toggle screen size
				m_pRenderer->GetPixelSize() == SCREENSIZE_SMALL ? m_pRenderer->SetPixelSize(SCREENSIZE_LARGE) : m_pRenderer->SetPixelSize(SCREENSIZE_SMALL);
				break;
			case OPTION_QUIT_GAME:				// Close game
				m_bExitApp = true;
				break;
			}

			bIsReturnPressed = true;
		}
	}
	else
	{
		bIsReturnPressed = false;
	}
}

void Game::UpdateGame(float deltaTime)
{
	UpdatePlayer(deltaTime);
	UpdateEnemies(deltaTime);

	UpdatePlayerProjectiles(deltaTime);
	UpdateEnemyProjectiles(deltaTime);

	for (SpriteAnimation& explosion : m_Explosions)
		if (explosion.Active())
			explosion.Update(deltaTime);
}

void Game::RenderGame()
{
	//render your game here
	player.Render(m_pRenderer);

	for (int i = 0; i < pEnemies.size(); i++)
		if (!pEnemies[i]->IsDestroyed())
			pEnemies[i]->Render(m_pRenderer);

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

	// Shoot projectile
	static bool bSpaceIsPressed = false;
	if (GetKeyState(VK_SPACE) < 0)
	{
		if (!bSpaceIsPressed)
		{
			if (player.TimeSinceLastShot() > player.ShootCooldownTime())
				player.Shoot(GetPlayerProjectile());
			bSpaceIsPressed = true;
		}
	}
	else
	{
		bSpaceIsPressed = false;
	}
}

void Game::UpdateEnemies(float deltaTime)
{
	for (int i = 0; i < pEnemies.size(); i++)
	{
		// Enemy is alive on screen
		if (!pEnemies[i]->IsDestroyed())
		{
			pEnemies[i]->Update(deltaTime);

			if (pEnemies[i]->FiredWeapon())
			{
				Projectile& enemyProjectile = GetEnemyProjectile();
				enemyProjectile.SetFiringState(true);
				enemyProjectile.SetPosition(pEnemies[i]->GetPosition().x, pEnemies[i]->GetPosition().y + (pEnemies[i]->GetSize().y / 2));
			}
		}
		// Enemy is ready to be spawned
		else
		{
			if (Random(1, 100) < 3)
			{
				pEnemies[i]->SetPosition(SCREEN_WIDTH, Random(SCREEN_BOUNDARY_TOP, SCREEN_HEIGHT - pEnemies[i]->GetSize().y));
				pEnemies[i]->ResetHealth();
				pEnemies[i]->SetActive();
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
			for (int j = 0; j < pEnemies.size(); j++)
			{
				if (!pEnemies[j]->IsDestroyed())
				{
					if (projectile.Collides(*pEnemies[j]))
					{
						pEnemies[j]->ApplyDamage(projectile.GetDamage());

						if (pEnemies[j]->IsDestroyed())
						{
							player.AddScore(pEnemies[j]->GetPoints());
							SetExplosion(*pEnemies[j]);
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
			explosion.SetPosition(plane.GetPosition().x + (plane.GetSize().x / 2) - (explosion.GetSize().x / 2), plane.GetPosition().y);
			explosion.SetFrame(0);
			explosion.SetActive(true);
			break;
		}
	}
}