#include "ProjectileManager.h"
//#include "Core/Renderer/ASCIIRenderer.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

ProjectileManager::ProjectileManager(int count, const int screenWidth) :
	m_BinPosition(50, 50),
	m_SCREEN_WIDTH(screenWidth)
{
	for (int i = 0; i < count; i++)
		m_ProjectileList.push_back(new Projectile(PROJECTILE_A, m_BinPosition));
}

ProjectileManager::~ProjectileManager()
{
	for (int i = 0; i < m_ProjectileList.size(); i++)
		delete m_ProjectileList[i];
}

void ProjectileManager::UpdateProjectiles()
{
	for (int i = 0; i < m_ProjectileList.size(); i++)
	{
		m_ProjectileList[i]->Update();

		if (m_ProjectileList[i]->GetPosition().x > m_SCREEN_WIDTH)
			ResetProjectile(*m_ProjectileList[i]);
	
	}		
}

void ProjectileManager::RenderProjectiles(ASCIIRenderer* pRenderer)
{
	for (int i = 0; i < m_ProjectileList.size(); i++)
		if (m_ProjectileList[i]->IsFiring())
			m_ProjectileList[i]->Render(pRenderer);
}

Projectile& ProjectileManager::GetProjectile()
{
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