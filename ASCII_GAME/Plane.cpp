#include "Plane.h"
#include "Core/Renderer/ASCIIRenderer.h"

void Plane::ApplyDamage(float damage)
{
	m_Health -= damage;

	SetPixelOverrideColour(ConsoleColour::BACKGROUND_BRIGHT_RED);

	if (m_Health <= 0.0f)
		m_bDestroyed = true;
}

void Plane::Shoot(Projectile& proj)
{
	proj.SetPosition(m_Position.x + m_Size.x, (m_Position.y + (m_Size.y >> 1)) - (proj.GetSize().y >> 1));
	proj.SetFiringState(true);
	m_ShootTimer.Reset();
}

void Plane::Render(ASCIIRenderer* pRenderer)
{
	Sprite::Render(pRenderer);
	ClearPixelOverrideColour();
}