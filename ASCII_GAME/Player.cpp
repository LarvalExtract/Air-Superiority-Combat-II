#include "Player.h"
#include "Projectile.h"

#include <Windows.h>

Player::Player() :
	m_Lives(3),
	m_Score(0)
{
	SetImage("player.tga");
	m_Health = 10.0f;
	m_Speed = 150.0f;
	m_ShootCooldown = 0.15f;
}

void Player::Update(float deltaTime)
{
	Plane::Update(deltaTime);

	if (GetKeyState(VK_DOWN) < 0)
		m_Position.y += m_Speed * deltaTime;

	if (GetKeyState(VK_UP) < 0)
		m_Position.y -= m_Speed * deltaTime;

	if (GetKeyState(VK_LEFT) < 0)
		m_Position.x -= m_Speed * deltaTime;

	if (GetKeyState(VK_RIGHT) < 0)
		m_Position.x += m_Speed * deltaTime;
}

bool Player::Fire()
{
	static bool bSpaceIsPressed = false;	// Prevents holding space to shoot

	if (GetKeyState(VK_SPACE) < 0)
	{
		if (!bSpaceIsPressed)
		{
			if (m_TimeSinceLastShot > 0.15f)
			{
				m_TimeSinceLastShot = 0.0f;
				bSpaceIsPressed = true;
				return true;
			}
		}
	}
	else
	{
		bSpaceIsPressed = false;
	}

	return false;
}