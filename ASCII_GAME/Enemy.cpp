#include "Enemy.h"

Enemy::Enemy(EnemyType type) :
	type(type)
{
	switch (type)
	{
	case ENEMY_LIGHT:
		SetImage("enemy1.tga");
		m_Health = 2;
		m_Speed = 135.0f;
		m_Points = 50;
		break;

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

	case ENEMY_HEAVY:
		SetImage("enemy5.tga");
		m_Health = 8;
		m_Speed = 50.0f;
		m_Points = 200;
		break;

	case ENEMY_GUNSHIP:
		SetImage("enemy6.tga");
		m_Health = 12;
		m_Speed = 35.0f;
		m_Points = 300;
		break;
	}
}

Enemy::~Enemy()
{

}

void Enemy::Update(float deltaTime)
{
	Plane::Update(deltaTime);

	switch (type)
	{
	case ENEMY_LIGHT:	break;
	case ENEMY_BIPLANE:	UpdateBiplane(deltaTime); break;
	case ENEMY_MEDIUM:	break;
	case ENEMY_HEAVY:	break;
	case ENEMY_GUNSHIP: break;
	}

	m_Position.x -= m_Speed * deltaTime;
}

void Enemy::UpdateBiplane(float deltaTime)
{
	static float counter = 0.0f;

	counter += 0.05f;
}