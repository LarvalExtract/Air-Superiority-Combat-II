#pragma once

#include "Projectile.h"
#include "Timer.h"

class Plane : public Sprite
{
public:
	virtual ~Plane() {};

	void ApplyDamage(float damage);
	bool IsDestroyed() const { return m_bDestroyed; }
	void Shoot(Projectile& proj);

	float TimeSinceLastShot() const { return m_ShootTimer.Elapsed(); }
	float ShootCooldownTime() const { return m_ShootCooldown; }

	void Render(ASCIIRenderer* pRenderer);

protected:
	Plane() : m_Health(0.0f), m_Speed(0.0f), m_bDestroyed(false), m_TookDamage(false) {};

	float m_Health;
	float m_Speed;
	bool m_bDestroyed;
	bool m_TookDamage;

	Timer<float> m_ShootTimer;
	float m_ShootCooldown;
};