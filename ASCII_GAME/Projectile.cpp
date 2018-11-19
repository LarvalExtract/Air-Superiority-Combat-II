#include "Projectile.h"

Projectile::Projectile(ProjectileType type) :
	m_Damage(1.0f),
	m_ProjectileSpeed(15.0f),
	m_bIsFiring(false)
{
	switch (type)
	{
	case PROJECTILE_PLAYER:
		SetImage("projectile1.tga");
		m_ProjectileSpeed = 15.0f;
		break;
	case PROJECTILE_ENEMY:
		SetImage("projectile2.tga");
		m_ProjectileSpeed = -13.0f;
		break;
	}
}

Projectile::~Projectile()
{

}

void Projectile::Update()
{
	m_Position.x += m_ProjectileSpeed;
}