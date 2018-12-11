#include "Game.h"
#include <windows.h>

#include "Core/Utils.h"
#include "Core/Renderer/ASCIIRenderer.h"

constexpr int HALF_SCREEN_WIDTH = SCREEN_WIDTH / 2;
constexpr int HALF_SCREEN_HEIGHT = SCREEN_HEIGHT / 2;

constexpr int SCREEN_BOUNDARY_TOP = 40;
constexpr int SCREEN_BOUNDARY_BOTTOM = SCREEN_HEIGHT - 5;
constexpr int SCREEN_BOUNDARY_LEFT = 0;
constexpr int SCREEN_BOUNDARY_RIGHT = 100;

constexpr int MAX_PLAYER_LIVES = 3;
constexpr int MAX_ENEMIES = 10;
constexpr int MAX_PLAYER_PROJECTILES = 15;
constexpr int MAX_ENEMY_PROJECTILES = 100;

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
	enemies(3),
	playerProjectiles(MAX_PLAYER_PROJECTILES, Projectile::s_playerProjectile),
	enemyProjectiles(MAX_ENEMY_PROJECTILES),
	enemySpawnChance(20)
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

	mainMenu.AddMenuItem(TGAFile("ui/menu_title.tga"));
	mainMenu.AddMenuItem(TGAFile("ui/menu_startgame.tga"));
	mainMenu.AddMenuItem(TGAFile("ui/menu_highscore.tga"));
	mainMenu.AddMenuItem(TGAFile("ui/menu_screensize.tga"));
	mainMenu.AddMenuItem(TGAFile("ui/menu_quit.tga"));
	mainMenu.SetPosition(HALF_SCREEN_WIDTH / 2, 20);
	mainMenu.SetItemSpacing(5);
	mainMenu.SetMinimumIndex(MAIN_MENU_START);
	mainMenu.AlignVertical();

	pauseMenu.AddMenuItem(TGAFile("ui/menu_paused.tga"));
	pauseMenu.AddMenuItem(TGAFile("ui/menu_resume.tga"));
	pauseMenu.AddMenuItem(TGAFile("ui/menu_screensize.tga"));
	pauseMenu.AddMenuItem(TGAFile("ui/menu_quit.tga"));
	pauseMenu.SetPosition(HALF_SCREEN_WIDTH / 2, 30);
	pauseMenu.SetItemSpacing(5);
	pauseMenu.SetMinimumIndex(PAUSE_MENU_RESUME);
	pauseMenu.AlignVertical();

	player.SetMaxLives(MAX_PLAYER_LIVES);

	// Position health icons at top middle of screen
	for (char i = 0; i < player.GetMaxLives(); i++)
	{
		m_PlayerLifeIcons.push_back(Sprite());

		int spacing = 7;
		int width = MAX_PLAYER_LIVES * playerHealthIcon.m_size.x;
		int totalWidth = width + ((MAX_PLAYER_LIVES - 1) * spacing);

		int xpos = i * playerHealthIcon.m_size.x + i * spacing;
		int ypos = 6;

		m_PlayerLifeIcons[i].SetTexture(playerHealthIcon);
		m_PlayerLifeIcons[i].SetPosition(HALF_SCREEN_WIDTH - (totalWidth / 2) + xpos, ypos);
	}

	explosionSprite.SetTextureArray(explosion);
	explosionSprite.SetFrameTime(0.035f);

	splash.SetTexture(TGAFile("splash.tga"));
	splash.SetPosition(HALF_SCREEN_WIDTH - splash.GetSize().x / 2, HALF_SCREEN_HEIGHT - splash.GetSize().y / 2);
	gameOver.SetTexture(TGAFile("ui/game_over.tga"));
	gameOver.SetPosition(HALF_SCREEN_WIDTH - gameOver.GetSize().x / 2, 10);

	highScore.SetTexture(TGAFile("ui/gameover_high_score.tga"));
	pressEsc.SetTexture(TGAFile("ui/gameover_press_esc.tga"));

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
	// Reset player state
	player.SetPosition(0, HALF_SCREEN_HEIGHT - player.GetSize().y / 2);
	player.SetLives(MAX_PLAYER_LIVES);
	player.SetScore(0);
	
	scoreDisplay.SetText(std::to_string(player.GetScore()));
	scoreDisplay.SetPosition(10, 5);

	// Reset player projectiles
	for (Projectile& proj : playerProjectiles)
		proj.SetFiringState(false);

	// Reset enemy projectiles
	for (Projectile& proj : enemyProjectiles)
		proj.SetFiringState(false);

	// Reset enemies
	enemies.resize(3);
	for (Enemy& enemy : enemies)
	{
		enemy.ResetHealth();
		enemy.SetActive(false);
	}

	roundTimer.Reset();
}

void Game::InitialiseGameOverScreen()
{
	int spacing = 8;
	int totalWidth = highScore.GetSize().x + spacing + scoreDisplay.GetSize().x;
	int highScorePos = HALF_SCREEN_WIDTH - totalWidth / 2;
	int scoreDisplayPos = highScorePos + highScore.GetSize().x + spacing;

	highScore.SetPosition(highScorePos, gameOver.GetPosition().y + gameOver.GetSize().y + 10);
	scoreDisplay.SetPosition(scoreDisplayPos, highScore.GetPosition().y);
	pressEsc.SetPosition(HALF_SCREEN_WIDTH - pressEsc.GetSize().x / 2, SCREEN_HEIGHT - 30);
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
	case E_GAME_STATE_GAME_OVER:	UpdateGameOverScreen(deltaTime); break;
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
	case E_GAME_STATE_GAME_OVER:	RenderGameOverScreen(); break;
	}

	//call this last
	m_pRenderer->Render();
}

void Game::UpdateMainMenu()
{
	if (OnKeyPressed(VK_RETURN))
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
	if (player.GetLives() <= 0)
	{
		m_GameState = E_GAME_STATE_GAME_OVER;
		InitialiseGameOverScreen();
		return;
	}

	if (OnKeyPressed(VK_ESCAPE))
	{
		m_GameState = E_GAME_STATE_PAUSED;
		pauseMenu.SetSelectionIndex(PAUSE_MENU_RESUME);
		return;
	}

	// Make the game harder every 30 seconds
	if (roundTimer.Elapsed() > 10.0f)
	{
		IncreaseDifficulty();
		roundTimer.Reset();
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

void Game::UpdateGameOverScreen(float deltaTime)
{
	if (OnKeyPressed(VK_ESCAPE))
	{
		m_GameState = E_GAME_STATE_MAIN_MENU;
	}

	static float flashTime = 0.0f;
	static bool bOverrideColour = true;

	flashTime += deltaTime;
	if (flashTime > 0.5f)
	{
		// Flashes the "PRESS ESC" sprite
		bOverrideColour ? pressEsc.SetPixelOverrideColour(BACKGROUND_YELLOW) : pressEsc.ClearPixelOverrideColour();

		bOverrideColour = !bOverrideColour;
		flashTime = 0.0f;
	}
}

void Game::UpdatePlayer(float deltaTime)
{
	player.Update(deltaTime);

	if (player.ShouldFire())
		player.Shoot(GetPlayerProjectile());

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
}

void Game::UpdateEnemies(float deltaTime)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		// Enemy is alive on screen
		if (enemies[i].IsActive())
		{
			enemies[i].Update(deltaTime);

			if (enemies[i].ShouldFire())
				enemies[i].Shoot(GetEnemyProjectile());

			// Check if enemy has escaped screen
			else if (enemies[i].GetPosition().x + enemies[i].GetSize().x < 0)
			{
				enemies[i].SetActive(false);
				player.DecrementLives();
			}			
		}
		// Enemy is ready to be spawned
		else
		{
			static float spawnTimer = 0.0f;
			spawnTimer += deltaTime;

			// Attempt to spawn an enemy every .25 seconds
			if (spawnTimer > 0.25f)
			{
				if (Random(1, 1000) < enemySpawnChance)
				{
					// Spawn enemy randomly on the Y axis
					enemies[i].SetPosition(SCREEN_WIDTH, Random(SCREEN_BOUNDARY_TOP, SCREEN_HEIGHT - enemies[i].GetSize().y));
					enemies[i].ResetHealth();
					enemies[i].SetActive();
				}

				spawnTimer = 0.0f;
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
				if (enemies[j].IsActive() && projectile.Collides(enemies[j]))
				{
					enemies[j].ApplyDamage(projectile.GetDamage());

					if (enemies[j].GetHealth() <= 0)
					{
						enemies[j].SetActive(false);

						player.AddScore(enemies[j].GetPoints());

						// Draw an explosion where the enemy was destroyed
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
			}

			else if (projectile.Collides(player))
			{
				player.ApplyDamage(1);
				//player.DecrementLives();

				projectile.SetFiringState(false);
			}
		}
	}
}

void Game::IncreaseDifficulty()
{
	if (enemies.size() < MAX_ENEMIES)
	{
		if (enemies.size() <= 5)
			enemies.push_back(Enemy(ENEMY_SPITFIRE));
		else if (enemies.size() <= 8)
			enemies.push_back(Enemy(ENEMY_BIPLANE));
		else if (enemies.size() <= 10)
			enemies.push_back(Enemy(ENEMY_GUNSHIP));
	}
	
}

void Game::RenderGame()
{
	scoreDisplay.Render(m_pRenderer);

	for (char i = 0; i < player.GetLives(); i++)
		m_PlayerLifeIcons[i].Render(m_pRenderer);

	player.Render(m_pRenderer);

	// Render enemies
	for (Enemy &enemy : enemies)
		if (enemy.IsActive())
			enemy.Render(m_pRenderer);

	// Render player projectiles
	for (Projectile &proj : playerProjectiles)
		if (proj.IsFiring())
			proj.Render(m_pRenderer);

	// Render enemy projectiles
	for (Projectile &proj : enemyProjectiles)
		if (proj.IsFiring())
			proj.Render(m_pRenderer);

	// Render explosions
	for (auto it = m_Explosions.begin(); it != m_Explosions.end(); it++)
		it->Render(m_pRenderer);
}

void Game::RenderGameOverScreen()
{
	splash.Render(m_pRenderer);

	gameOver.Render(m_pRenderer);
	highScore.Render(m_pRenderer);
	scoreDisplay.Render(m_pRenderer);
	pressEsc.Render(m_pRenderer);
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

	// Prevents accidental registration of multiple key presses from the user
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