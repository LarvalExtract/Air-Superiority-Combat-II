#include "Enemy.h"
#include "Core/Utils.h"
#include "Projectile.h"

Texture s_spitfireTexture(TGAFile("enemy3.tga"));
Texture s_biplaneTexture(TGAFile("enemy2.tga"));
Texture s_gunshipTexture(TGAFile("enemy5.tga"));

Enemy::Enemy(EnemyType type) :
	m_Type(type),
	m_biplaneRounds(0),
	m_spitfireFireChance(0),
	m_gunshipFireRate(0.0f)
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
		m_biplaneRounds = 3;	// Biplanes fire in 3 round bursts
		break;

	case ENEMY_SPITFIRE:
		SetTexture(s_spitfireTexture);
		SetMaxHealth(300.0f);
		m_Speed = 60.0f;
		m_Points = 10;
		m_spitfireFireChance = 15;	// Spitfire has a 15% chance of firing
		break;

	case ENEMY_GUNSHIP:
		SetTexture(s_gunshipTexture);
		SetMaxHealth(1000.0f);
		m_Speed = 35.0f;
		m_Points = 50;
		m_gunshipFireRate = 3.0f;	// Enemy gunship fires a shot every 3 seconds
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
		if (Random(1, 100) < m_spitfireFireChance)
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
		if (m_biplaneRounds > 0)
		{
			if (bulletTime > 0.25f)
			{
				m_biplaneRounds--;
				bulletTime = 0.0f;
				return true;
			}
			return false;
		}
		else
		{
			m_TimeSinceLastShot = 0.0f;
			m_biplaneRounds = 3;
		}
	}

	return false;
}

bool Enemy::FireGunship()
{
	if (m_TimeSinceLastShot > m_gunshipFireRate)
	{
		m_TimeSinceLastShot = 0.0f;
		return true;
	}

	m_TimeSinceLastShot += s_deltaTime;
	return false;
}