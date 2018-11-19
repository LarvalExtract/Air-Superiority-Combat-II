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

	void MoveUp() { m_Position.y -= m_Speed; }
	void MoveDown() { m_Position.y += m_Speed; }
	void MoveRight() { m_Position.x += m_Speed; }
	void MoveLeft() { m_Position.x -= m_Speed; }

	void AddScore(unsigned int score) { m_Score += score; }

private:
	unsigned char m_Lives;
	unsigned int m_Score;
};