#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "Core/Renderer/Sprite.h"

class DigitDisplay
{
public:
	DigitDisplay() : m_Position(0, 0), m_Size(0, 0), m_Spacing(1) {}

	static void Initialise();

	void SetPosition(int x, int y);
	void SetSpacing(int spacing);

	void SetNumber(unsigned int number);
	void Render(ASCIIRenderer* pRenderer);

	Vec2<int> GetSize();

private:
	static bool bInitialised;
	static std::unordered_map<char, Texture> characters;

	std::vector<Sprite> m_Sprites;
	std::string m_Text;
	Vec2<int> m_Position;
	Vec2<int> m_Size;
	int m_Spacing;
};