#include "Plane.h"
#include "Core/Renderer/ASCIIRenderer.h"

void Plane::ApplyDamage(unsigned char damage)
{
	m_Health -= damage;
	m_HitTime = 0.0f;

	SetPixelOverrideColour(ConsoleColour::BACKGROUND_BRIGHT_RED);

	if (m_Health <= 0)
		m_bDestroyed = true;
}

void Plane::Update(float deltaTime)
{
	m_HitTime += deltaTime;
	m_TimeSinceLastShot += deltaTime;

	if (m_HitTime > 0.1f)
		ClearPixelOverrideColour();
}

void Plane::Shoot(Projectile& proj)
{
	proj.SetPosition(m_Position.x + m_Size.x, (m_Position.y + (m_Size.y >> 1)) - (proj.GetSize().y >> 1));
	proj.SetFiringState(true);
	m_TimeSinceLastShot = 0.0f;
}