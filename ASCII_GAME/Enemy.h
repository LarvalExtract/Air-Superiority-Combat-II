#pragma once

#include "Plane.h"

enum EnemyType
{
	ENEMY_LIGHT,
	ENEMY_BIPLANE,
	ENEMY_MEDIUM,
	ENEMY_HEAVY,
	ENEMY_GUNSHIP
};

class Enemy : public Plane
{
public:
	Enemy(EnemyType type = ENEMY_HEAVY);
	~Enemy();

	void Update();

	unsigned int GetPoints() const { return m_Points; }

private:


	unsigned int m_Points;
	EnemyType type;
};