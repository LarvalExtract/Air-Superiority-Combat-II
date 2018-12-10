#include "Plane.h"
#include "Core/Renderer/ASCIIRenderer.h"

void Plane::ApplyDamage(unsigned char damage)
{
	m_Health -= damage;
	m_HitTime = 0.0f;

	SetPixelOverrideColour(ConsoleColour::BACKGROUND_BRIGHT_RED);
}

void Plane::Update(float deltaTime)
{
	m_HitTime += deltaTime;
	m_TimeSinceLastShot += deltaTime;

	if (m_HitTime > 0.05f)
		ClearPixelOverrideColour();
}