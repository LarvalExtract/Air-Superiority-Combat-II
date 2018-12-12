#include "Player.h"
#include "Projectile.h"

#include <Windows.h>

Player::Player() :
	m_Lives(0),
	m_Score(0),
	m_MaxLives(3)
{
	SetTexture(TGAFile("player.tga"));
	m_MaxHealth = 500.0f;
	m_Health = m_MaxHealth;
	m_Speed = 150.0f;
	m_ShootCooldown = 0.15f;
}

void Player::SetLives(unsigned char lives)
{
	if (lives >= 0 && lives <= m_MaxLives)
		m_Lives = lives;
}

void Player::SetScore(unsigned int score)
{
	m_Score = score;
}

void Player::IncrementLives()
{
	m_Lives++;

	if (m_Lives > m_MaxLives)
		m_Lives = m_MaxLives;
}

void Player::DecrementLives()
{
	m_Lives--;

	if (m_Lives < 0)
		m_Lives = 0;
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

bool Player::ShouldFire()
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

void Player::Shoot(Projectile &proj)
{
	proj.SetPosition(m_Position.x + GetSize().x, (GetPosition().y + (GetSize().y / 2)) - proj.GetSize().y / 2);
	proj.SetVelocity(Vec2<float>(400.0f, 0.0f));
	proj.SetFiringState(true);
}