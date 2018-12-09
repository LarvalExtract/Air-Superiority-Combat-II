#pragma once

#include "Core/Renderer/Sprite.h"

class Plane : public Sprite
{
public:
	virtual ~Plane() {};

	void Update(float deltaTime);

	virtual bool Fire() = 0;

	void ApplyDamage(unsigned char damage);

	bool IsDestroyed() const { return m_bDestroyed; }
	void SetActive(bool value = true) { m_bDestroyed = !value; }

	float TimeSinceLastShot() const { return m_TimeSinceLastShot; }
	float ShootCooldownTime() const { return m_ShootCooldown; }

protected:
	Plane() : m_Health(0), m_Speed(0.0f), m_bDestroyed(false), m_TookDamage(false), m_TimeSinceLastShot(0.0f) {}

	char m_Health;
	float m_Speed;
	float m_HitTime;

	bool m_bDestroyed;
	bool m_TookDamage;

	float m_ShootCooldown;
	float m_TimeSinceLastShot;
};