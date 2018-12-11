#pragma once

#include <map>
#include <string>
#include <vector>

#include "Core/Renderer/Sprite.h"

class SpriteText
{
public:
	SpriteText() : m_bInitialised(false), m_Position(0, 0), m_Size(0, 0), m_Spacing(1) {}

	void Initialise();
	void SetPosition(int x, int y);
	void SetSpacing(int spacing);

	void SetText(const std::string &text);
	void Render(ASCIIRenderer* pRenderer);

	Vec2<int> GetSize();

private:
	bool m_bInitialised;
	static std::map<char, Texture> characters;

	std::vector<Sprite> m_Sprites;
	std::string m_Text;
	Vec2<int> m_Position;
	Vec2<int> m_Size;
	int m_Spacing;
};