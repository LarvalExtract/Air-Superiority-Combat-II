#include "Player.h"
#include "Projectile.h"

Player::Player() :
	Sprite("player.tga"),
	m_Lives(3),
	m_MovementSpeed(5),
	m_ShootWaitTime(0.35)
{

}

void Player::MoveUp()
{
	m_Position.y -= m_MovementSpeed;
}

void Player::MoveDown()
{
	m_Position.y += m_MovementSpeed;
}

void Player::MoveRight()
{
	m_Position.x += 6;
}

void Player::MoveLeft()
{
	m_Position.x -= 6;
}

void Player::Shoot(Projectile &proj)
{
	proj.SetPosition(m_Position.x + m_Size.x, (m_Position.y + (m_Size.y / 2)) - (proj.GetSize().y / 2));
	proj.SetFiringState(true);
	shootTimer.Reset();
}