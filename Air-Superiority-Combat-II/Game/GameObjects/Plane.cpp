#include "Plane.h"
#include "Core/Renderer/ASCIIRenderer.h"

float Plane::s_deltaTime = 0.0f;

void Plane::ApplyDamage(float damage)
{
	m_Health -= damage;
	m_HitTime = 0.0f;

	SetPixelOverrideColour(ConsoleColour::BACKGROUND_BRIGHT_RED);

	m_HealthDisplay.SetValue(m_Health);
}

void Plane::SetPosition(float x, float y)
{
	Sprite::SetPosition(x, y);

	m_HealthDisplay.SetPosition((m_Position.x + GetSize().x / 2) - m_HealthDisplay.GetSize().x / 2, m_Position.y - 3);
}

void Plane::SetMaxHealth(float value)
{
	m_MaxHealth = value;
	m_HealthDisplay.SetMaximumValue(value);
}

void Plane::Update(float deltaTime)
{
	s_deltaTime = deltaTime;

	m_HitTime += s_deltaTime;

	if (m_HitTime > 0.05f)
		ClearPixelOverrideColour();

	m_HealthDisplay.SetPosition((m_Position.x + GetSize().x / 2) - m_HealthDisplay.GetSize().x / 2, m_Position.y - 3);
}

void Plane::RenderHealthDisplay(ASCIIRenderer* pRenderer)
{
	m_HealthDisplay.Render(pRenderer);
}

void Plane::ResetHealth()
{
	m_Health = m_MaxHealth;
	m_HealthDisplay.SetValue(m_MaxHealth);
}