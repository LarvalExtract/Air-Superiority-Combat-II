#pragma once

#include "Core/Renderer/Sprite.h"
#include "Timer.h"

class Projectile;

class Player : public Sprite
{
public:
	Player();

	unsigned char Lives() const { return m_Lives; }
	unsigned int Score() const { return m_Score; }
	unsigned char GetDamage() const { return m_Damage;}

	double TimeSinceLastShot() const { return shootTimer.Elapsed(); }
	double ShootCooldownTime() const { return m_ShootWaitTime; }

	void SetLives(unsigned char lives) { m_Lives = lives; }
	void SetSpeed(unsigned char speed) { m_MovementSpeed = speed; }

	void IncrementLives() { m_Lives++; }
	void DecrementLives() { m_Lives--; }

	void MoveUp();
	void MoveDown();
	void MoveRight();
	void MoveLeft();

	void Shoot(Projectile &proj);

	void AddScore(unsigned int score) { m_Score += score; }

private:
	unsigned char m_Lives;
	unsigned char m_MovementSpeed;
	unsigned int m_Score;
	unsigned char m_Damage;

	double m_ShootWaitTime;
	Timer shootTimer;
};