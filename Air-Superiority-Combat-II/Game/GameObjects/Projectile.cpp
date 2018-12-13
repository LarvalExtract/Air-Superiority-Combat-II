#include "Projectile.h"

CONSOLE_PIXEL enemyProjectile3Pixels[] = { {0, BACKGROUND_BRIGHT_RED}, {0, BACKGROUND_YELLOW}, {0, BACKGROUND_YELLOW} };

Texture Projectile::s_playerProjectile(TGAFile("projectile1.tga"));
Texture Projectile::s_enemyProjectile1(TGAFile("projectile1_left.tga"));
Texture Projectile::s_enemyProjectile2(TGAFile("projectile2.tga"));
Texture Projectile::s_enemyProjectile3(enemyProjectile3Pixels, Vec2<short>(3, 1));

Projectile::Projectile() :
	m_Damage(0.0f),
	m_bIsFiring(false)
{
	
}

Projectile::Projectile(const Texture& texture) :
	m_Damage(0.0f),
	m_bIsFiring(false)
{
	SetTexture(texture);
}

Projectile::~Projectile()
{

}

void Projectile::Update(float deltaTime)
{
	m_Position.x += m_Velocity.x * deltaTime;
}