#pragma once

#include "Core/Renderer/Sprite.h"
#include "../../HealthDisplay.h"

class Plane : public Sprite
{
public:
	virtual ~Plane() {};

	virtual bool ShouldFire() = 0;

	void Update(float deltaTime);
	void RenderHealthDisplay(ASCIIRenderer* pRenderer);

	void ApplyDamage(float damage);
	float GetHealth() const { return m_Health; }
	void SetMaxHealth(float value);
	void ResetHealth();

	void SetActive(bool value = true) { m_bActive = value; }
	bool IsActive() const { return m_bActive; }

protected:
	Plane() : m_Health(0), m_Speed(0.0f), m_bActive(false), m_TookDamage(false), m_TimeSinceLastShot(0.0f) {}

	HealthDisplay m_HealthDisplay;

	float m_Health;
	float m_MaxHealth;
	float m_Speed;
	float m_HitTime;

	bool m_bActive;
	bool m_TookDamage;

	float m_ShootCooldown;
	float m_TimeSinceLastShot;
};