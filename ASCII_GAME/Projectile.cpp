#include "Projectile.h"

Texture Projectile::s_playerProjectile(TGAFile("projectile1.tga"));
Texture Projectile::s_enemyProjectile(TGAFile("projectile2.tga"));

Projectile::Projectile(ProjectileType type) :
	m_Damage(1),
	m_ProjectileSpeed(0.0f),
	m_bIsFiring(false)
{
	switch (type)
	{
	case PROJECTILE_PLAYER:
		SetTexture(s_playerProjectile);
		m_ProjectileSpeed = 400.0f;
		break;
	case PROJECTILE_ENEMY:
		SetTexture(s_enemyProjectile);
		m_ProjectileSpeed = -150.0f;
		break;
	}
}

Projectile::~Projectile()
{

}

void Projectile::Update(float deltaTime)
{
	m_Position.x += m_ProjectileSpeed * deltaTime;
}