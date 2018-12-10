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
	bool ShouldFire() override;
	void SetProjectile(Projectile &proj);

	void ResetHealth();

	unsigned int GetPoints() const { return m_Points; }

	void SetEnemyType(EnemyType type) { m_Type = type; }

private:
	void UpdateBiplane(float deltaTime);

	bool FireSpitfire();
	bool FireBiplane();
	bool FireGunship();

	unsigned int m_Points;
	EnemyType m_Type;

	static float s_DeltaTime;

	static char s_biplaneRounds;
	static char s_mediumFireChance;
	static float s_gunshipFireRate;
};