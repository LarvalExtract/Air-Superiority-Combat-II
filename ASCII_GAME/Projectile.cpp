#include "Projectile.h"

Projectile::Projectile(ProjectileType type, Vector2 &pos) :
	m_Damage(1),
	m_ProjectileSpeed(15),
	m_bIsFiring(false)
{
	m_Position = pos;

	switch (type)
	{
	case PROJECTILE_PLAYER:
		SetImage("projectile1.tga");
		m_ProjectileSpeed = 15;
		break;
	case PROJECTILE_ENEMY:
		SetImage("projectile2.tga");
		m_ProjectileSpeed = -13;
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