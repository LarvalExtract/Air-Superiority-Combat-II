#pragma once

#include "Plane.h"

class Player : public Plane
{
public:
	Player();
	~Player() {}

	unsigned char GetLives() const { return m_Lives; }
	unsigned int GetScore() const { return m_Score; }

	void SetLives(unsigned char lives) { m_Lives = lives; }

	void IncrementLives() { m_Lives++; }
	void DecrementLives() { m_Lives--; }

	void MoveUp(float deltaTime) { m_Position.y -= m_Speed * deltaTime; }
	void MoveDown(float deltaTime) { m_Position.y += m_Speed * deltaTime; }
	void MoveRight(float deltaTime) { m_Position.x += m_Speed * deltaTime; }
	void MoveLeft(float deltaTime) { m_Position.x -= m_Speed * deltaTime; }

	void AddScore(unsigned int score) { m_Score += score; }

private:
	unsigned char m_Lives;
	unsigned int m_Score;
};