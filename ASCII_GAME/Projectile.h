#pragma once

#include "Core/Renderer/Sprite.h"

enum ProjectileType
{
	PROJECTILE_PLAYER,
	PROJECTILE_ENEMY
};

class Projectile : public Sprite
{
public:
	Projectile(ProjectileType type);
	~Projectile();

	void Update(float deltaTime);
	void SetFiringState(bool state) { m_bIsFiring = state; }
	void SetSpeed(float speed) { m_ProjectileSpeed = speed; }

	bool IsFiring() const { return m_bIsFiring; }

	unsigned char GetDamage() const { return m_Damage; }

private:
	unsigned char m_Damage;
	float m_ProjectileSpeed;
	bool m_bIsFiring;

	static Texture s_playerProjectile;
	static Texture s_enemyProjectile;
};