#include "SpriteText.h"
#include "Core/Renderer/ASCIIRenderer.h"

std::map<char, SpriteCharacter*> SpriteText::characters;

void SpriteText::Initialise()
{
	typedef std::pair<char, SpriteCharacter*> pair;

	SpriteCharacter* num0 = new SpriteCharacter("numbers/0.tga", 0, 0, 0);
	SpriteCharacter* num1 = new SpriteCharacter("numbers/1.tga", 0, 0, 0);
	SpriteCharacter* num2 = new SpriteCharacter("numbers/2.tga", 0, 0, 0);
	SpriteCharacter* num3 = new SpriteCharacter("numbers/3.tga", 0, 0, 0);
	SpriteCharacter* num4 = new SpriteCharacter("numbers/4.tga", 0, 0, 0);
	SpriteCharacter* num5 = new SpriteCharacter("numbers/5.tga", 0, 0, 0);
	SpriteCharacter* num6 = new SpriteCharacter("numbers/6.tga", 6, 0, 0);
	SpriteCharacter* num7 = new SpriteCharacter("numbers/7.tga", 0, 0, 0);
	SpriteCharacter* num8 = new SpriteCharacter("numbers/8.tga", 6, 0, 0);
	SpriteCharacter* num9 = new SpriteCharacter("numbers/9.tga", 0, 0, 0);

	characters.insert(pair('0', num0));
	characters.insert(pair('1', num1));
	characters.insert(pair('2', num2));
	characters.insert(pair('3', num3));
	characters.insert(pair('4', num4));
	characters.insert(pair('5', num5));
	characters.insert(pair('6', num6));
	characters.insert(pair('7', num7));
	characters.insert(pair('8', num8));
	characters.insert(pair('9', num9));
}

void SpriteText::SetText(const std::string &text)
{
	m_Sprites.clear();
	m_Sprites.reserve(text.size());

	for (int i = 0; i < text.size(); i++)
		m_Sprites.push_back(*characters[text[i]]);
}

void SpriteText::SetPosition(int x, int y)
{
	if (m_Sprites.empty())
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