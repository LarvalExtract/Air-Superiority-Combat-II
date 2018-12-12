#include "Plane.h"
#include "Core/Renderer/ASCIIRenderer.h"

void Plane::ApplyDamage(float damage)
{
	m_Health -= damage;
	m_HitTime = 0.0f;

	SetPixelOverrideColour(ConsoleColour::BACKGROUND_BRIGHT_RED);

	m_HealthDisplay.SetValue(m_Health);
}

void Plane::Update(float deltaTime)
{
	m_HitTime += deltaTime;
	m_TimeSinceLastShot += deltaTime;

	if (m_HitTime > 0.05f)
		ClearPixelOverrideColour();

	m_HealthDisplay.SetPosition((m_Position.x + GetSize().x / 2) - m_HealthDisplay.GetSize().x / 2, m_Position.y - 3);
}

void Plane::RenderHealthDisplay(ASCIIRenderer* pRenderer)
{
	m_HealthDisplay.Render(pRenderer);
}
