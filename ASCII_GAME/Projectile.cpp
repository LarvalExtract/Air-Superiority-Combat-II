#include "Projectile.h"

Projectile::Projectile(ProjectileType type, Vector2 &pos) :
	m_Damage(1),
	m_ProjectileSpeed(15),
	m_bIsFiring(false)
{
	m_Position = pos;

	switch (type)
	{
	case PROJECTILE_A: SetImage("projectile1.tga"); break;
	case PROJECTILE_B: SetImage("projectile2.tga"); break;
	}
}

Projectile::~Projectile()
{

}

void Projectile::Update()
{
	if (m_bIsFiring)
	{
		m_Position.x += m_ProjectileSpeed;
	}
}