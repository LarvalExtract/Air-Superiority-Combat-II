#pragma once

#include "Core/Renderer/Sprite.h"

enum ProjectileType
{
	PROJECTILE_A,
	PROJECTILE_B
};

class Projectile : public Sprite
{
public:
	Projectile(ProjectileType type, Vector2 &pos);
	~Projectile();

	void Update();
	void SetFiringState(bool state) { m_bIsFiring = state; }

	bool IsFiring() const { return m_bIsFiring; }

	unsigned char GetDamage() const { return m_Damage; }

private:
	unsigned char m_Damage;
	unsigned char m_ProjectileSpeed;
	bool m_bIsFiring;
};