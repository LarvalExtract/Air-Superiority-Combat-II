#pragma once

#include "Plane.h"

class Projectile;

class Player : public Plane
{
public:
	Player();
	~Player() {}

	bool ShouldFire();
	void Shoot(Projectile &proj);

	unsigned char GetLives() const { return m_Lives; }
	unsigned int GetScore() const { return m_Score; }
	unsigned char GetMaxLives() const { return m_MaxLives; }

	void SetLives(unsigned char lives);
	void SetMaxLives(unsigned char maxLives) { m_MaxLives = maxLives; }
	void SetScore(unsigned int score);

	void IncrementLives();
	void DecrementLives();

	void AddScore(unsigned int score) { m_Score += score; }

private:
	char m_Lives;
	unsigned int m_Score;
	unsigned char m_MaxLives;
};