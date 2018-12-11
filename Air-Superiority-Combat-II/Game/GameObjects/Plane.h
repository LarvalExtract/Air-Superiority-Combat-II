#pragma once

#include "Core/Renderer/Sprite.h"

class Plane : public Sprite
{
public:
	virtual ~Plane() {};

	void Update(float deltaTime);

	virtual bool ShouldFire() = 0;

	void ApplyDamage(unsigned char damage);

	char GetHealth() const { return m_Health; }

	void SetActive(bool value = true) { m_bActive = value; }
	bool IsActive() const { return m_bActive; }

	float TimeSinceLastShot() const { return m_TimeSinceLastShot; }
	float ShootCooldownTime() const { return m_ShootCooldown; }

protected:
	Plane() : m_Health(0), m_Speed(0.0f), m_bActive(false), m_TookDamage(false), m_TimeSinceLastShot(0.0f) {}

	char m_Health;
	float m_Speed;
	float m_HitTime;

	bool m_bActive;
	bool m_TookDamage;

	float m_ShootCooldown;
	float m_TimeSinceLastShot;
};