#include "DigitDisplay.h"
#include "Core/Renderer/ASCIIRenderer.h"

std::unordered_map<char, Texture> DigitDisplay::characters;
bool DigitDisplay::bInitialised = false;

void DigitDisplay::Initialise()
{
	if (!bInitialised)
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

		bInitialised = true;
	}
}

void DigitDisplay::SetNumber(unsigned int number)
{
	if (!bInitialised)
		return;

	m_Text = std::to_string(number);

	m_Sprites.clear();
	m_Sprites.reserve(m_Text.size());

	for (int i = 0; i < m_Text.size(); i++)
	{
		m_Sprites.push_back(Sprite());
		m_Sprites[i].SetTexture(characters[m_Text[i]]);
	}

	SetPosition(m_Position.x, m_Position.y);
}

void DigitDisplay::SetPosition(int x, int y)
{
	if (!bInitialised)
		return;

	m_Position.x = x;
	m_Position.y = y;

	if (m_Sprites.empty())
		return;

	m_Sprites[0].SetPosition(x, y);

	for (int i = 1; i < m_Sprites.size(); i++)
	{
		int xpos = m_Sprites[i - 1].GetPosition().x + m_Sprites[i - 1].GetSize().x + m_Spacing;
		int ypos = y;

		m_Sprites[i].SetPosition(xpos, ypos);
	}
}

void DigitDisplay::SetSpacing(int spacing)
{
	if (!bInitialised)
		return;

	m_Spacing = spacing;

	SetPosition(m_Position.x, m_Position.y);
}

void DigitDisplay::Render(ASCIIRenderer* pRenderer)
{
	if (!bInitialised)
		return;

	for (int i = 0; i < m_Sprites.size(); i++)
	{
		m_Sprites[i].Render(pRenderer);
	}
}

Vec2<int> DigitDisplay::GetSize()
{
	if (!bInitialised)
		return Vec2<int>(0, 0);

	m_Size.x = 0;
	m_Size.y = 0;

	for (Sprite &character : m_Sprites)
	{
		if (character.GetSize().y > m_Size.y)
			m_Size.y = character.GetSize().y;

		m_Size.x += character.GetSize().x;
	}

	m_Size.x += m_Spacing * (m_Sprites.size() - 1);

	return m_Size;
}