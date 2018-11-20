#include "Player.h"
#include "Projectile.h"

Player::Player() :
	m_Lives(3),
	m_Score(0)
{
	SetImage("player.tga");
	m_Health = 10.0f;
	m_Speed = 150.0f;
	m_ShootCooldown = 0.3f;
}