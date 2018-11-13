#include "Enemy.h"

Enemy::Enemy(EnemyType type) :
	m_Health(0),
	m_Speed(0),
	m_bDestroy(false)
{
	Initialise(type);
}

Enemy::~Enemy()
{

}

bool Enemy::Initialise(EnemyType type)
{
	switch (type)
	{
	case ENEMY_LIGHT: 
		SetImage("enemy1.tga");
		m_Health = 1;
		m_Speed = 9;
		break;

	case ENEMY_BIPLANE:
		SetImage("enemy2.tga");
		m_Health = 2;
		m_Speed = 7;
		break;

	case ENEMY_MEDIUM:
		SetImage("enemy3.tga");
		m_Health = 3;
		m_Speed = 6;
		break;

	case ENEMY_HEAVY:
		SetImage("enemy5.tga");
		m_Health = 6;
		m_Speed = 4;
		break;

	case ENEMY_GUNSHIP:
		SetImage("enemy6.tga");
		m_Health = 12;
		m_Speed = 3;
		break;
	}

	return true;
}

void Enemy::MoveLeft()
{
	m_Position.x -= m_Speed;
}

void Enemy::ApplyDamage(unsigned char damage)
{
	m_Health -= damage;

	if (m_Health <= 0)
		m_bDestroy = true;
}