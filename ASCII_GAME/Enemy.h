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

	void MoveLeft();
	void ApplyDamage(unsigned char damage);

private:
	int m_Health;
	unsigned char m_Speed;
	bool m_bDestroy;

	bool Initialise(EnemyType type);
};