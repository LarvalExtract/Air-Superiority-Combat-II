#pragma once

#include "Plane.h"

enum EnemyType
{
	ENEMY_BIPLANE,
	ENEMY_MEDIUM,
	ENEMY_GUNSHIP
};

class Enemy : public Plane
{
public:
	Enemy(EnemyType type);
	~Enemy();

	void Update(float deltaTime);
	bool FiredWeapon();

	void ResetHealth();

	unsigned int GetPoints() const { return m_Points; }

private:
	void UpdateBiplane(float deltaTime);

	bool FireMedium();
	bool FireBiplane();
	bool FireGunship();

	unsigned int m_Points;
	EnemyType type;

	static float s_DeltaTime;

	static char s_biplaneRounds;
	static char s_mediumFireChance;
	static float s_gunshipFireRate;
};