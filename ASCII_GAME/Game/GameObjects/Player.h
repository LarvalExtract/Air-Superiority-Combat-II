#pragma once

#include "Plane.h"

class Player : public Plane
{
public:
	Player();
	~Player() {}

	void Update(float deltaTime);

	bool Fire() override;

	unsigned char GetLives() const { return m_Lives; }
	unsigned int GetScore() const { return m_Score; }
	unsigned char GetMaxLives() const { return m_MaxLives; }

	void SetLives(unsigned char lives);
	void SetMaxLives(unsigned char maxLives) { m_MaxLives = maxLives; }
	void SetScore(unsigned int score);

	void IncrementLives();
	void DecrementLives();

	void MoveUp(float deltaTime) { m_Position.y -= m_Speed * deltaTime; }
	void MoveDown(float deltaTime) { m_Position.y += m_Speed * deltaTime; }
	void MoveRight(float deltaTime) { m_Position.x += m_Speed * deltaTime; }
	void MoveLeft(float deltaTime) { m_Position.x -= m_Speed * deltaTime; }

	void AddScore(unsigned int score) { m_Score += score; }

private:
	char m_Lives;
	unsigned int m_Score;
	unsigned char m_MaxLives;
};