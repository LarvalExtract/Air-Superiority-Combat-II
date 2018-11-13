#pragma once

#include "Projectile.h"
#include "Core/Maths/Vector2.h"

#include <vector>

class ProjectileManager
{
public:
	ProjectileManager(int count, const int screenWidth);
	~ProjectileManager();

	void UpdateProjectiles();
	void RenderProjectiles(ASCIIRenderer* pRenderer);

	Projectile& GetProjectile();

private:
	void ResetProjectile(Projectile &proj);

	std::vector<Projectile*> m_ProjectileList;
	std::vector<Projectile*> m_EnemyProjectileList;
	Vector2 m_BinPosition;

	const int m_SCREEN_WIDTH;
};