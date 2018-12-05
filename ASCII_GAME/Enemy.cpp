#include "Enemy.h"
#include "Core/Utils.h"

float Enemy::s_DeltaTime = 0.0f;

char	Enemy::s_biplaneRounds = 3;
char	Enemy::s_mediumFireChance = 10;
float	Enemy::s_gunshipFireRate = 2.0f;

Enemy::Enemy(EnemyType type) :
	type(type)
{
	switch (type)
	{
	case ENEMY_BIPLANE:
		SetImage("enemy2.tga");
		m_Health = 4;
		m_Speed = 75.0f;
		m_Points = 100;
		break;

	case ENEMY_MEDIUM:
		SetImage("enemy3.tga");
		m_Health = 5;
		m_Speed = 60.0f;
		m_Points = 150;
		break;

	case ENEMY_GUNSHIP:
		SetImage("enemy6.tga");
		m_Health = 12;
		m_Speed = 35.0f;
		m_Points = 300;
		break;
	}

	m_bDestroyed = true;
}

Enemy::~Enemy()
{

}

void Enemy::ResetHealth()
{
	switch (type)
	{
	case ENEMY_MEDIUM: m_Health = 5; break;
	case ENEMY_BIPLANE: m_Health = 4; break;
	case ENEMY_GUNSHIP: m_Health = 12; break;
	}
}

void Enemy::Update(float deltaTime)
{
	Plane::Update(deltaTime);

	Enemy::s_DeltaTime = deltaTime;

	switch (type)
	{
	case ENEMY_BIPLANE:	UpdateBiplane(deltaTime); break;
	case ENEMY_MEDIUM:	break;
	case ENEMY_GUNSHIP: break;
	}

	m_Position.x -= m_Speed * deltaTime;
}

bool Enemy::Fire()
{
	switch (type)
	{
	case ENEMY_MEDIUM:	return FireMedium();
	case ENEMY_BIPLANE:	return FireBiplane();
	case ENEMY_GUNSHIP: return FireGunship();
	}

	return false;
}

bool Enemy::FireMedium()
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

	return false;
}

bool Enemy::FireBiplane()
{
	static float bulletTime = 0.0f;

	bulletTime += s_DeltaTime;

	if (m_TimeSinceLastShot > 2.0f)
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

	return false;
}

void Enemy::UpdateBiplane(float deltaTime)
{
	static float counter = 0.0f;

	counter += 0.05f;
}