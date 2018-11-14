#pragma once

#include "Projectile.h"
#include "Core/Maths/Vector2.h"

#include "Player.h"
#include "Enemy.h"

#include <vector>

class ProjectileManager
{
public:
	ProjectileManager(int count, const int screenWidth);
	~ProjectileManager();

	void UpdatePlayerProjectiles(Enemy &enemy);
	void UpdateEnemyProjectiles(Player &player);

	void RenderProjectiles(ASCIIRenderer* pRenderer);

	Projectile& GetProjectile();

private:
	void ResetProjectile(Projectile &proj);

	std::vector<Projectile*> m_EnemyProjectiles;
	std::vector<Projectile*> m_PlayerProjectiles;

	Vector2 m_BinPosition;

	const int m_SCREEN_WIDTH;

	const int maxPlayerProjectiles;
	const int maxEnemyProjectiles;
};