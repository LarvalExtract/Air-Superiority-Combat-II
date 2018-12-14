#pragma once

#include "Core/Renderer/Sprite.h"

class HealthDisplay : public Sprite
{
public:
	HealthDisplay();
	~HealthDisplay();

	void SetValue(float value);
	void SetMaximumValue(float value);

private:
	float m_Value;
	float m_MaxValue;
};