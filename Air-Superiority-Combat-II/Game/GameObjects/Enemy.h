#pragma once

#include "Plane.h"

enum EnemyType
{
	ENEMY_BIPLANE,
	ENEMY_SPITFIRE,
	ENEMY_GUNSHIP
};

class Projectile;

class Enemy : public Plane
{
public:
	Enemy(EnemyType type = ENEMY_SPITFIRE);
	~Enemy();

	void Update(float deltaTime);
	bool ShouldFire();
	void Shoot(Projectile &proj);

	unsigned int GetPoints() const { return m_Points; }

	void SetEnemyType(EnemyType type);

private:
	bool FireSpitfire();
	bool FireBiplane();
	bool FireGunship();

	unsigned int m_Points;
	EnemyType m_Type;

	char m_biplaneRounds;
	char m_spitfireFireChance;
	float m_gunshipFireRate;
};