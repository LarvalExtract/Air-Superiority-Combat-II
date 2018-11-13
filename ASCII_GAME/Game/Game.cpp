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

#define VK_LEFT		0x25
#define VK_RIGHT	0x27
#define VK_SPACE	0x20       

Game::Game() :
	m_pRenderer(NULL),
	m_bInitialised(false),
	m_bExitApp(false),
	m_Checkpoint(0),
	m_ScreenProjectiles(30, SCREEN_WIDTH)
{
	
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

	pEnemies.push_back(new Enemy(ENEMY_LIGHT));
	pEnemies.push_back(new Enemy(ENEMY_BIPLANE));
	pEnemies.push_back(new Enemy(ENEMY_MEDIUM));
	pEnemies.push_back(new Enemy(ENEMY_HEAVY));
	pEnemies.push_back(new Enemy(ENEMY_GUNSHIP));

	player.SetPosition(0, SCREEN_HEIGHT / 2 - player.GetSize().y / 2);
	
	pEnemies[0]->SetPosition(SCREEN_WIDTH, 40);
	pEnemies[1]->SetPosition(SCREEN_WIDTH, 60);
	pEnemies[2]->SetPosition(SCREEN_WIDTH, 80);
	pEnemies[3]->SetPosition(SCREEN_WIDTH, 100);
	pEnemies[4]->SetPosition(SCREEN_WIDTH, 120);

	m_bInitialised = true;
}

void Game::InitialiseRenderer()
{
	m_pRenderer = new ASCIIRenderer();

	m_pRenderer->Initialise(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Game::Run()
{
	while (!m_bExitApp)
	{
		Update();

		Render();

		//Sleep(5);
	}
}

void Game::Update()
{
	ProcessInputs();

	m_ScreenProjectiles.UpdateProjectiles();

	for (int i = 0; i < pEnemies.size(); i++)
		pEnemies[i]->MoveLeft();
}

void Game::Render()
{
	//call this first
	m_pRenderer->ClearScreen();	

	//render your game here
	player.Render(m_pRenderer);

	m_ScreenProjectiles.RenderProjectiles(m_pRenderer);

	for (int i = 0; i < pEnemies.size(); i++)
		pEnemies[i]->Render(m_pRenderer);

	//call this last
	m_pRenderer->Render();
}

void Game::ProcessInputs()
{
	if (GetKeyState(VK_ESCAPE) < 0)
	{
		m_bExitApp = true;
		return;
	}

	// Move player up
	if (GetKeyState(VK_UP) < 0)
		if (player.GetPosition().y > SCREEN_BOUNDARY_TOP)
			player.MoveUp();

	// Move player down
	if (GetKeyState(VK_DOWN) < 0)
		if (player.GetPosition().y + player.GetSize().y < SCREEN_BOUNDARY_BOTTOM)
			player.MoveDown();
	
	// Move player right
	if (GetKeyState(VK_RIGHT) < 0)
		if (player.GetPosition().x < SCREEN_BOUNDARY_RIGHT)
			player.MoveRight();

	// Move player right
	if (GetKeyState(VK_LEFT) < 0)
		if (player.GetPosition().x > SCREEN_BOUNDARY_LEFT)
			player.MoveLeft();

	// Shoot projectile
	if (GetKeyState(VK_SPACE) < 0)
		if (player.TimeSinceLastShot() > player.ShootCooldownTime())
			player.Shoot(m_ScreenProjectiles.GetProjectile());
}