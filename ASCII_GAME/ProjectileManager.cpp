#include "ProjectileManager.h"
//#include "Core/Renderer/ASCIIRenderer.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

ProjectileManager::ProjectileManager(int count, const int screenWidth) :
	m_BinPosition(50, 50),
	m_SCREEN_WIDTH(screenWidth),
	maxPlayerProjectiles(20),
	maxEnemyProjectiles(40)
{
	for (int i = 0; i < maxPlayerProjectiles; i++)
		m_PlayerProjectiles.push_back(new Projectile(PROJECTILE_PLAYER, m_BinPosition));

	for (int i = 0; i < maxEnemyProjectiles; i++)
		m_EnemyProjectiles.push_back(new Projectile(PROJECTILE_ENEMY, m_BinPosition));
}

ProjectileManager::~ProjectileManager()
{
	for (int i = 0; i < m_PlayerProjectiles.size(); i++)
		delete m_PlayerProjectiles[i];

	for (int i = 0; i < m_EnemyProjectiles.size(); i++)
		delete m_EnemyProjectiles[i];
}

void ProjectileManager::UpdatePlayerProjectiles(Enemy &enemy)
{
	for (int i = 0; i < m_PlayerProjectiles.size(); i++)
	{
		if (m_PlayerProjectiles[i]->IsFiring())
		{
			m_PlayerProjectiles[i]->Update();

			if (m_PlayerProjectiles[i]->Collides(enemy))
			{
				
			}
		}
	}
}

void ProjectileManager::UpdateEnemyProjectiles(Player &player)
{
	for (int i = 0; i < m_EnemyProjectiles.size(); i++)
	{
		if (m_EnemyProjectiles[i]->IsFiring())
		{
			m_EnemyProjectiles[i]->Update();

			if (m_EnemyProjectiles[i]->Collides(player))
			{
				ResetProjectile(*m_EnemyProjectiles[i]);
				player.DecrementLives();
			}
		}
	}
}

void ProjectileManager::RenderProjectiles(ASCIIRenderer* pRenderer)
{
	for (int i = 0; i < m_PlayerProjectiles.size(); i++)
		if (m_PlayerProjectiles[i]->IsFiring())
			m_PlayerProjectiles[i]->Render(pRenderer);

	for (int i = 0; i < m_EnemyProjectiles.size(); i++)
		if (m_EnemyProjectiles[i]->IsFiring())
			m_EnemyProjectiles[i]->Render(pRenderer);
}

Projectile& ProjectileManager::GetProjectile()
{
	// Returns the first available projectile
	for (int i = 0; i < m_ProjectileList.size(); i++)
		if (!m_ProjectileList[i]->IsFiring())
		{			
			return *m_ProjectileList[i];
		}

	//Projectile *projectile = new Projectile;
	//m_ProjectileList.push_back(projectile);
	//
	//return *projectile;
}

void ProjectileManager::ResetProjectile(Projectile &proj)
{
	proj.SetFiringState(false);
	proj.SetPosition(m_BinPosition);
}