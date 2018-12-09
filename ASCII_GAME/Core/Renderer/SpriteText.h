#pragma once

#include <map>
#include <string>
#include <vector>

#include "Core/Renderer/Sprite.h"

class SpriteText
{
public:
	SpriteText() : m_bInitialised(false), m_Position(0, 0) {}

	void Initialise();
	void SetPosition(int x, int y);

	void SetText(const std::string &text);
	void Render(ASCIIRenderer* pRenderer);

private:
	bool m_bInitialised;
	static std::map<char, Texture> characters;

	std::vector<Sprite> m_Sprites;
	Vec2<int> m_Position;
};