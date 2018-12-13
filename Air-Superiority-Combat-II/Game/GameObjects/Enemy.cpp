#include "Enemy.h"
#include "Core/Utils.h"
#include "Projectile.h"

char	Enemy::s_biplaneRounds = 3;
char	Enemy::s_mediumFireChance = 15;
float	Enemy::s_gunshipFireRate = 2.0f;

Texture s_spitfireTexture(TGAFile("enemy3.tga"));
Texture s_biplaneTexture(TGAFile("enemy2.tga"));
Texture s_gunshipTexture(TGAFile("enemy5.tga"));

Enemy::Enemy(EnemyType type) :
	m_Type(type)
{
	SetEnemyType(type);
}

Enemy::~Enemy()
{

}

void Enemy::SetEnemyType(EnemyType type)
{
	m_Type = type;

	switch (m_Type)
	{
	case ENEMY_BIPLANE:
		SetTexture(s_biplaneTexture);
		SetMaxHealth(500.0f);
		m_Speed = 75.0f;
		m_Points = 20;
		break;

	case ENEMY_SPITFIRE:
		SetTexture(s_spitfireTexture);
		SetMaxHealth(300.0f);
		m_Speed = 60.0f;
		m_Points = 10;
		break;

	case ENEMY_GUNSHIP:
		SetTexture(s_gunshipTexture);
		SetMaxHealth(1000.0f);
		m_Speed = 35.0f;
		m_Points = 50;
		break;
	}

	ResetHealth();
}

void Enemy::Update(float deltaTime)
{
	Plane::Update(deltaTime);

	m_Position.x -= m_Speed * deltaTime;
}

bool Enemy::ShouldFire()
{
	switch (m_Type)
	{
	case ENEMY_SPITFIRE:	return FireSpitfire();
	case ENEMY_BIPLANE:		return FireBiplane();
	case ENEMY_GUNSHIP:		return FireGunship();
	default:				return false;
	}
}

void Enemy::Shoot(Projectile &proj)
{
	switch (m_Type)
	{
	case ENEMY_SPITFIRE:	
		proj.SetTexture(Projectile::s_enemyProjectile1);
		proj.SetVelocity(Vec2<float>(-150.0f, 0.0f));
		proj.SetDamage(90.0f);
		break;
	case ENEMY_BIPLANE:		
		proj.SetTexture(Projectile::s_enemyProjectile2);
		proj.SetVelocity(Vec2<float>(-170.0f, 0.0f));
		proj.SetDamage(40.0f);
		break;
	case ENEMY_GUNSHIP:		
		proj.SetTexture(Projectile::s_enemyProjectile2);
		proj.SetVelocity(Vec2<float>(-125.0f, 0.0f));
		proj.SetDamage(140.0f);
		break;
	}

	proj.SetPosition(m_Position.x, m_Position.y + GetSize().y / 2 - (proj.GetSize().y / 2));
	proj.SetFiringState(true);
}

bool Enemy::FireSpitfire()
{
	if (m_TimeSinceLastShot > 0.5f)
	{
		if (Random(1, 100) < s_mediumFireChance)
		{
			m_TimeSinceLastShot = 0.0f;
			return true;
		}
		m_TimeSinceLastShot = 0.0f;
	}

	m_TimeSinceLastShot += s_deltaTime;
	return false;
}

bool Enemy::FireBiplane()
{
	static float bulletTime = 0.0f;

	bulletTime += s_deltaTime;
	m_TimeSinceLastShot += s_deltaTime;

	if (m_TimeSinceLastShot > 3.0f)
	{
		if (s_biplaneRounds > 0)
		{
			if (bulletTime > 0.25f)
			{
				s_biplaneRounds--;
				bulletTime = 0.0f;
				return true;
			}
			return false;
		}
		else
		{
			m_TimeSinceLastShot = 0.0f;
			s_biplaneRounds = 3;
		}
	}

	return false;
}

bool Enemy::FireGunship()
{
	if (m_TimeSinceLastShot > s_gunshipFireRate)
	{
		m_TimeSinceLastShot = 0.0f;
		return true;
	}
	else
	{
		m_TimeSinceLastShot += s_deltaTime;
		return false;
	}
}