#pragma once

#include <map>
#include <string>
#include <vector>

#include "SpriteCharacter.h"

class ASCIIRenderer;

class SpriteText
{
public:

	void Initialise();
	void SetPosition(int x, int y);

	void SetText(const std::string &text);
	void Render(ASCIIRenderer* pRenderer);

private:
	static std::map<char, SpriteCharacter*> characters;
	std::vector<SpriteCharacter> m_Sprites;
	Vec2<int> m_Position;
};