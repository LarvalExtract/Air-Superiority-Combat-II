#pragma once

#include "Core/Renderer/Sprite.h"

class Projectile : public Sprite
{
public:
	Projectile();
	Projectile(const Texture& texture);
	~Projectile();

	void Update(float deltaTime);
	void SetFiringState(bool state) { m_bIsFiring = state; }
	void SetVelocity(Vec2<float> velocity) { m_Velocity = velocity; }

	bool IsFiring() const { return m_bIsFiring; }

	float GetDamage() const { return m_Damage; }

private:
	float m_Damage;
	Vec2<float> m_Velocity;
	bool m_bIsFiring;

public:
	static Texture s_playerProjectile;
	static Texture s_enemyProjectile1;
	static Texture s_enemyProjectile2;
	static Texture s_enemyProjectile3;
};