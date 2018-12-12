#include "HealthDisplay.h"

#include "Core/Renderer/ASCIIRenderer.h"

wchar_t fill = L'\u2588';

CONSOLE_PIXEL sprite[] = 
{ 
	{0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true},
	{0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true}, {0, BACKGROUND_BRIGHT_RED, true},
};
Vec2<short> barSize(32, 2);

HealthDisplay::HealthDisplay() :
	m_Value(0.0f),
	m_MaxValue(0.0f)
{
	Initialise(sprite, barSize);
}

HealthDisplay::~HealthDisplay()
{

}

void HealthDisplay::SetValue(float value)
{
	m_Value = value;

	int fill = (barSize.x / m_MaxValue) * value;

	for (char i = 0; i < barSize.x; i++)
	{
		if (i < fill)
		{
			m_Texture.m_pixelData[i].char_info.Attributes = BACKGROUND_BRIGHT_RED;
			m_Texture.m_pixelData[i + barSize.x].char_info.Attributes = BACKGROUND_BRIGHT_RED;
		}
		else
		{
			m_Texture.m_pixelData[i].char_info.Attributes = BACKGROUND_RED;
			m_Texture.m_pixelData[i + barSize.x].char_info.Attributes = BACKGROUND_RED;
		}
	}
}

void HealthDisplay::SetMaximumValue(float value)
{
	m_MaxValue = value;
}