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
	Projectile(ProjectileType type, Vector2 &pos);
	~Projectile();

	void Update();
	void SetFiringState(bool state) { m_bIsFiring = state; }
	void SetSpeed(char speed) { m_ProjectileSpeed = speed; }

	bool IsFiring() const { return m_bIsFiring; }

	unsigned char GetDamage() const { return m_Damage; }

private:
	unsigned char m_Damage;
	char m_ProjectileSpeed;
	bool m_bIsFiring;
};