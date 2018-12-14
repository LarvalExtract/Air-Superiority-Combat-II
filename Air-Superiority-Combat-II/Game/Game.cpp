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
constexpr int MAX_ENEMIES = 50;
constexpr int MAX_PLAYER_PROJECTILES = 15;
constexpr int MAX_ENEMY_PROJECTILES = 100;

#define VK_LEFT		0x25
#define VK_RIGHT	0x27
#define VK_SPACE	0x20       

bool Game::m_Keys[7] = { false };

Texture playerHealthIcon(TGAFile("health_icon.tga"));
TextureArray explosionFrames(TGAFile("explosion.tga"), 6);

enum E_MAIN_MENU_OPTIONS : unsigned char
{
	MAIN_MENU_START = 1,
	MAIN_MENU_START_ENDLESS,
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
	m_bPlayerDied(false),
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

	// Delete all texture data from memory
	Texture::CleanUpAllTextures();
}

void Game::Initialise()
{
	if (m_bInitialised)
		return;

	m_pRenderer = new ASCIIRenderer();
	m_pRenderer->Initialise(SCREEN_WIDTH, SCREEN_HEIGHT);

	DigitDisplay::Initialise();

	mainMenu.AddMenuItem(TGAFile("ui/menu_title.tga"));
	mainMenu.AddMenuItem(TGAFile("ui/menu_startgame.tga"));
	mainMenu.AddMenuItem(TGAFile("ui/menu_startendless.tga"));
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

	explosionSprite.SetTextureArray(explosionFrames);
	explosionSprite.SetFrameTime(0.035f);

	splash.SetTexture(TGAFile("splash.tga"));
	gameOver.SetTexture(TGAFile("ui/game_over.tga"));
	youWin.SetTexture(TGAFile("ui/gameover_you_win.tga"));
	highScore.SetTexture(TGAFile("ui/gameover_high_score.tga"));
	pressEsc.SetTexture(TGAFile("ui/gameover_press_esc.tga"));

	m_bInitialised = true;
}

void Game::Run()
{
	float lastTime = 0.0f;
	float newTime = gameTimer.Elapsed();

	while (!m_bExitApp)
	{
		newTime = gameTimer.Elapsed();
		m_deltaTime = newTime - lastTime;
		lastTime = newTime;

		Update(m_deltaTime);

		Render();
	}
}

void Game::InitialiseGame()
{
	// Reset player state
	m_bPlayerDied = false;
	player.SetActive(true);
	player.SetPosition(0, HALF_SCREEN_HEIGHT - player.GetSize().y / 2);
	player.SetMaxLives(MAX_PLAYER_LIVES);
	player.SetLives(MAX_PLAYER_LIVES);
	player.SetScore(0);
	player.ResetHealth();

	m_KillCount = 0;
	
	scoreDisplay.SetNumber(player.GetScore());
	scoreDisplay.SetPosition(10, 5);

	killCountDisplay.SetNumber(m_KillCount);
	killCountDisplay.SetPosition(SCREEN_WIDTH - 100, 5);

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

	m_Explosions.clear();

	roundTimer.Reset();
}

void Game::InitialiseGameOverScreen()
{
	// Position "scoreDisplay" and "highScore" sprites in center of screen
	int spacing = 8;
	int totalWidth = highScore.GetSize().x + spacing + scoreDisplay.GetSize().x;
	int highScorePos = HALF_SCREEN_WIDTH - totalWidth / 2;
	int scoreDisplayPos = highScorePos + highScore.GetSize().x + spacing;

	gameOver.SetPosition(HALF_SCREEN_WIDTH - gameOver.GetSize().x / 2, 10);
	highScore.SetPosition(highScorePos, gameOver.GetPosition().y + gameOver.GetSize().y + 10);
	scoreDisplay.SetPosition(scoreDisplayPos, highScore.GetPosition().y);
	pressEsc.SetPosition(HALF_SCREEN_WIDTH - pressEsc.GetSize().x / 2, SCREEN_HEIGHT - 30);
	splash.SetPosition(HALF_SCREEN_WIDTH - splash.GetSize().x / 2, HALF_SCREEN_HEIGHT - splash.GetSize().y / 2);
}

void Game::InitialiseYouWinScreen()
{
	// Position "scoreDisplay" and "highScore" sprites in center of screen
	int spacing = 8;
	int totalWidth = highScore.GetSize().x + spacing + scoreDisplay.GetSize().x;
	int highScorePos = HALF_SCREEN_WIDTH - totalWidth / 2;
	int scoreDisplayPos = highScorePos + highScore.GetSize().x + spacing;

	youWin.SetPosition(HALF_SCREEN_WIDTH - youWin.GetSize().x / 2, 60);
	highScore.SetPosition(highScorePos, youWin.GetPosition().y + youWin.GetSize().y + 10);
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
	case E_GAME_STATE_YOU_WIN:		UpdateYouWinScreen(deltaTime);
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
	case E_GAME_STATE_YOU_WIN:		RenderYouWinScreen(); break;
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
			m_bEndlessMode = false;
			break;
		case MAIN_MENU_START_ENDLESS:		// Start endless game mode
			InitialiseGame();
			m_GameState = E_GAME_STATE_IN_GAME;
			m_bEndlessMode = true;
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
	if (m_bEndlessMode)
	{
		if (player.GetLives() <= 0)	// End game condition for endless mode
		{
			m_GameState = E_GAME_STATE_GAME_OVER;
			InitialiseGameOverScreen();
			return;
		}
	}
	else
	{
		if (player.GetLives() <= 0)
		{
			m_GameState = E_GAME_STATE_GAME_OVER;
			InitialiseGameOverScreen();
			return;
		}
		else if (m_KillCount == MAX_ENEMIES) // Player can win by defeating 50 enemies
		{
			m_GameState = E_GAME_STATE_YOU_WIN;
			InitialiseYouWinScreen();
			return;
		}
	}

	// Pause the game if user pressed esc
	if (OnKeyPressed(VK_ESCAPE))
	{
		m_GameState = E_GAME_STATE_PAUSED;
		pauseMenu.SetSelectionIndex(PAUSE_MENU_RESUME);
		return;
	}

	UpdateExplosionFrames(deltaTime);

	// If player was destroyed, wait 2 seconds before respawning player and resuming action
	if (m_bPlayerDied)
	{
		RespawnPlayer(2.0f);
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

	static int lastScore = 0;
	if (player.GetScore() != lastScore)
		scoreDisplay.SetNumber(player.GetScore());
	lastScore = player.GetScore();

	static int lastKillCount = 0;
	if (m_KillCount != lastKillCount)
		killCountDisplay.SetNumber(m_KillCount);
	lastKillCount = m_KillCount;
}

void Game::UpdateGameOverScreen(float deltaTime)
{
	if (OnKeyPressed(VK_ESCAPE))
	{
		m_GameState = E_GAME_STATE_MAIN_MENU;
	}

	UpdateExplosionFrames(deltaTime);

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

void Game::UpdateYouWinScreen(float deltaTime)
{
	UpdateGameOverScreen(deltaTime);
}

void Game::UpdatePlayer(float deltaTime)
{
	// Move player down
	if (GetKeyState(VK_DOWN) < 0)
		player.SetPosition(player.GetPosition().x, player.GetPosition().y + player.GetSpeed() * deltaTime);

	// Move player up
	if (GetKeyState(VK_UP) < 0)
		player.SetPosition(player.GetPosition().x, player.GetPosition().y - player.GetSpeed() * deltaTime);

	// Move player left
	if (GetKeyState(VK_LEFT) < 0)
		player.SetPosition(player.GetPosition().x - player.GetSpeed() * deltaTime, player.GetPosition().y);

	// Move player right
	if (GetKeyState(VK_RIGHT) < 0)
		player.SetPosition(player.GetPosition().x + player.GetSpeed() * deltaTime, player.GetPosition().y);

	if (player.ShouldFire())
		player.Shoot(GetProjectile(playerProjectiles));

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

	player.Update(deltaTime);
}

void Game::UpdateEnemies(float deltaTime)
{
	for (Enemy &enemy : enemies)
	{
		// Enemy is alive on screen
		if (enemy.IsActive())
		{
			enemy.Update(deltaTime);

			if (enemy.ShouldFire())
				enemy.Shoot(GetProjectile(enemyProjectiles));

			// Check if enemy has reached left side of screen
			else if (enemy.GetPosition().x < 0)
			{
				player.DecrementLives();
				player.SetActive(false);
				ResetEnemies();
				m_bPlayerDied = true;
				return;
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
					enemy.SetPosition(SCREEN_WIDTH, Random(SCREEN_BOUNDARY_TOP, SCREEN_HEIGHT - enemy.GetSize().y));
					enemy.ResetHealth();
					enemy.SetActive();
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
			for (Enemy &enemy : enemies)
			{
				if (enemy.IsActive() && projectile.Collides(enemy))
				{
					enemy.ApplyDamage(projectile.GetDamage());

					// Enemy was destroyed
					if (enemy.GetHealth() <= 0.0f)
					{
						enemy.SetActive(false);
						player.AddScore(enemy.GetPoints());
						DrawExplosion(enemy.GetPosition());
						m_KillCount++;
					}

					projectile.SetFiringState(false);

					// Projectile hit an enemy and was destroyed, no need to check if it's colliding with any more enemies
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
				player.ApplyDamage(projectile.GetDamage());
				
				if (player.GetHealth() <= 0.0f)
				{
					player.DecrementLives();
					player.SetActive(false);
					DrawExplosion(player.GetPosition());
					ResetEnemies();
					m_bPlayerDied = true;
				}

				projectile.SetFiringState(false);
			}
		}
	}
}

void Game::IncreaseDifficulty()
{
	if (enemies.size() <= 5)
		enemies.push_back(Enemy(ENEMY_SPITFIRE));
	else if (enemies.size() <= 8)
		enemies.push_back(Enemy(ENEMY_BIPLANE));
	else if (enemies.size() <= 10)
		enemies.push_back(Enemy(ENEMY_GUNSHIP));
	
}

void Game::RenderGame()
{
	scoreDisplay.Render(m_pRenderer);
	killCountDisplay.Render(m_pRenderer);

	// Render player life icons
	for (char i = 0; i < player.GetLives(); i++)
		m_PlayerLifeIcons[i].Render(m_pRenderer);

	// Render enemies
	for (Enemy &enemy : enemies)
		if (enemy.IsActive())
			enemy.Render(m_pRenderer);

	// Render enemy health displays
	for (Enemy &enemy : enemies)
		if (enemy.IsActive())
			enemy.RenderHealthDisplay(m_pRenderer);

	// Render player projectiles
	for (Projectile &proj : playerProjectiles)
		if (proj.IsFiring())
			proj.Render(m_pRenderer);

	// Render player
	if (player.IsActive())
	{
		player.Render(m_pRenderer);
		player.RenderHealthDisplay(m_pRenderer);
	}

	// Render enemy projectiles
	for (Projectile &proj : enemyProjectiles)
		if (proj.IsFiring())
			proj.Render(m_pRenderer);

	// Render explosions
	for (SpriteAnimation &explosion : m_Explosions)
		explosion.Render(m_pRenderer);
}

void Game::RenderGameOverScreen()
{
	splash.Render(m_pRenderer);

	gameOver.Render(m_pRenderer);
	highScore.Render(m_pRenderer);
	scoreDisplay.Render(m_pRenderer);
	pressEsc.Render(m_pRenderer);
}

void Game::RenderYouWinScreen()
{
	// Render player
	RenderGame();

	youWin.Render(m_pRenderer);
	highScore.Render(m_pRenderer);
	scoreDisplay.Render(m_pRenderer);
	pressEsc.Render(m_pRenderer);
}

Projectile& Game::GetProjectile(std::vector<Projectile> &projectiles)
{
	for (Projectile& projectile : projectiles)
		if (!projectile.IsFiring())
			return projectile;

	// All projectiles were active, add a new one and return that
	projectiles.push_back(Projectile());
	return projectiles.back();
}

void Game::ResetGameObjects()
{
	ResetEnemies();
	ResetAllProjectiles();

	player.ResetHealth();
	player.SetPosition(0, HALF_SCREEN_HEIGHT);
}

void Game::ResetEnemies()
{
	for (Enemy &enemy : enemies)
	{
		enemy.ResetHealth();
		enemy.SetActive(false);
	}
}

void Game::ResetAllProjectiles()
{
	for (Projectile &proj : playerProjectiles)
		proj.SetFiringState(false);

	for (Projectile &proj : enemyProjectiles)
		proj.SetFiringState(false);
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

void Game::DrawExplosion(Vec2<float> &position)
{
	m_Explosions.push_back(explosionFrames);

	m_Explosions.back().SetPosition(position);
	m_Explosions.back().SetFrameTime(0.035f);
}

void Game::UpdateExplosionFrames(float deltaTime)
{
	for (auto it = m_Explosions.begin(); it != m_Explosions.end(); )
	{
		it->Update(deltaTime);

		// Remove explosion after it's played once
		it->GetLoopCount() > 0 ? it = m_Explosions.erase(it) : it++;
	}
}

void Game::RespawnPlayer(float time)
{
	if (!m_bPlayerDied)
		return;

	static float waitTime = 0.0f;

	waitTime += m_deltaTime;
	if (waitTime > time)
	{
		player.SetActive();
		player.ResetHealth();
		player.SetPosition(0, HALF_SCREEN_HEIGHT);

		if (!m_bEndlessMode)
			player.SetScore(0);

		ResetAllProjectiles();

		m_bPlayerDied = false;
		waitTime = 0.0f;
	}
}