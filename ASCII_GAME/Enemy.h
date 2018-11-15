#pragma once

#include "Core/Renderer/Sprite.h"

enum EnemyType
{
	ENEMY_LIGHT,
	ENEMY_BIPLANE,
	ENEMY_MEDIUM,
	ENEMY_HEAVY,
	ENEMY_GUNSHIP
};

class Enemy : public Sprite
{
public:
	Enemy(EnemyType type = ENEMY_HEAVY);
	~Enemy();

	char GetHealth() const { return m_Health; }
	unsigned int GetPoints() const { return m_Points; }
	bool IsDestroyed() const { return m_bDestroyed;  }

	void MoveLeft();
	void ApplyDamage(unsigned char damage);

private:
	char m_Health;
	unsigned char m_Speed;
	bool m_bDestroyed;
	unsigned int m_Points;

	bool Initialise(EnemyType type);
};