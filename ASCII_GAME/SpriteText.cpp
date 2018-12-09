#include "SpriteText.h"
#include "Core/Renderer/ASCIIRenderer.h"

std::map<char, Texture> SpriteText::characters;

void SpriteText::Initialise()
{
	if (!m_bInitialised)
	{
		typedef std::pair<char, Texture> pair;

		characters.insert(pair('0', TGAFile("numbers/0.tga")));
		characters.insert(pair('1', TGAFile("numbers/1.tga")));
		characters.insert(pair('2', TGAFile("numbers/2.tga")));
		characters.insert(pair('3', TGAFile("numbers/3.tga")));
		characters.insert(pair('4', TGAFile("numbers/4.tga")));
		characters.insert(pair('5', TGAFile("numbers/5.tga")));
		characters.insert(pair('6', TGAFile("numbers/6.tga")));
		characters.insert(pair('7', TGAFile("numbers/7.tga")));
		characters.insert(pair('8', TGAFile("numbers/8.tga")));
		characters.insert(pair('9', TGAFile("numbers/9.tga")));

		m_bInitialised = true;
	}
}

void SpriteText::SetText(const std::string &text)
{
	m_Sprites.clear();
	m_Sprites.reserve(text.size());

	for (int i = 0; i < text.size(); i++)
	{
		m_Sprites.push_back(Sprite());
		m_Sprites[i].SetTexture(characters[text[i]]);
	}

	SetPosition(m_Position.x, m_Position.y);
}

void SpriteText::SetPosition(int x, int y)
{
	m_Position.x = x;
	m_Position.y = y;

	if (m_Sprites.empty())
		return;

	m_Sprites[0].SetPosition(x, y);

	for (int i = 1; i < m_Sprites.size(); i++)
	{
		int xpos = m_Sprites[i - 1].GetPosition().x + m_Sprites[i - 1].GetSize().x + 1;
		int ypos = y;

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