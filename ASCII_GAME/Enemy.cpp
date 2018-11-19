#include "Enemy.h"

Enemy::Enemy(EnemyType type) :
	type(type)
{
	switch (type)
	{
	case ENEMY_LIGHT:
		SetImage("enemy1.tga");
		m_Health = 1;
		m_Speed = 9;
		m_Points = 50;
		break;

	case ENEMY_BIPLANE:
		SetImage("enemy2.tga");
		m_Health = 2;
		m_Speed = 7;
		m_Points = 100;
		break;

	case ENEMY_MEDIUM:
		SetImage("enemy3.tga");
		m_Health = 3;
		m_Speed = 6;
		m_Points = 150;
		break;

	case ENEMY_HEAVY:
		SetImage("enemy5.tga");
		m_Health = 6;
		m_Speed = 4;
		m_Points = 200;
		break;

	case ENEMY_GUNSHIP:
		SetImage("enemy6.tga");
		m_Health = 12;
		m_Speed = 3;
		m_Points = 300;
		break;
	}
}

Enemy::~Enemy()
{

}

void Enemy::Update()
{
	switch (type)
	{
	case ENEMY_LIGHT:
		break;

	case ENEMY_BIPLANE:
		break;

	case ENEMY_MEDIUM:
		break;

	case ENEMY_HEAVY:
		break;

	case ENEMY_GUNSHIP:
		break;
	}

	m_Position.x -= m_Speed;
}