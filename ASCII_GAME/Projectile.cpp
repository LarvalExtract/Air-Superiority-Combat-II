#include "Projectile.h"

Projectile::Projectile(ProjectileType type) :
	m_Damage(1),
	m_ProjectileSpeed(0.0f),
	m_bIsFiring(false)
{
	switch (type)
	{
	case PROJECTILE_PLAYER:
		SetImage("projectile1.tga");
		m_ProjectileSpeed = 400.0f;
		break;
	case PROJECTILE_ENEMY:
		SetImage("projectile2.tga");
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