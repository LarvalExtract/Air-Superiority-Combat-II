#include "SpriteText.h"
#include "Core/Renderer/ASCIIRenderer.h"

std::map<char, SpriteCharacter*> SpriteText::m_Characters;

void SpriteText::Initialise()
{
	typedef std::pair<char, SpriteCharacter*> pair;
	SpriteText::m_Characters.insert(pair('0', new SpriteCharacter("numbers/0.tga", 0, 0, 0)));
	SpriteText::m_Characters.insert(pair('1', new SpriteCharacter("numbers/1.tga", 0, 0, 0)));
	SpriteText::m_Characters.insert(pair('2', new SpriteCharacter("numbers/2.tga", 0, 0, 0)));
	SpriteText::m_Characters.insert(pair('3', new SpriteCharacter("numbers/3.tga", 0, 0, 0)));
	SpriteText::m_Characters.insert(pair('4', new SpriteCharacter("numbers/4.tga", 0, 0, 0)));
	SpriteText::m_Characters.insert(pair('5', new SpriteCharacter("numbers/5.tga", 0, 0, 0)));
	SpriteText::m_Characters.insert(pair('6', new SpriteCharacter("numbers/6.tga", 6, 0, 0)));
	SpriteText::m_Characters.insert(pair('7', new SpriteCharacter("numbers/7.tga", 0, 0, 0)));
	SpriteText::m_Characters.insert(pair('8', new SpriteCharacter("numbers/8.tga", 6, 0, 0)));
	SpriteText::m_Characters.insert(pair('9', new SpriteCharacter("numbers/9.tga", 0, 0, 0)));
}

void SpriteText::SetText(const std::string &text)
{
	m_Text = text;

	m_Sprites.clear();
	m_Sprites.reserve(text.size());

	for (int i = 0; i < m_Text.size(); i++)
		m_Sprites.push_back(*m_Characters[m_Text[i]]);
}

void SpriteText::SetPosition(int x, int y)
{
	if (m_Text.empty())
		return;

	m_Sprites[0].SetPosition(x, y);

	for (int i = 1; i < m_Sprites.size(); i++)
	{
		int xpos = m_Sprites[i - 1].GetPosition().x + m_Sprites[i - 1].GetSize().x + 1;
		int ypos = y - m_Sprites[i].GetLeading();

		m_Sprites[i].SetPosition(xpos, ypos);
	}
}

void SpriteText::Render(ASCIIRenderer* pRenderer)
{
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		m_Sprites[i].Render(pRenderer);
	}
}