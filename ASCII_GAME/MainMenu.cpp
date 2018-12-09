#include "MainMenu.h"
#include "Core/Renderer/ASCIIRenderer.h"

#include <Windows.h>

enum MenuSprites
{
	MENU_TITLE,
	MENU_START_GAME,
	MENU_HIGH_SCORE,
	MENU_SCREEN_SIZE,
	MENU_QUIT_GAME
};

Menu::Menu() :
	m_SelectionColour(ConsoleColour::BACKGROUND_YELLOW),
	m_Position(0, 0),
	m_Size(0, 0),
	m_ItemSpacing(0),
	m_MenuIndex(0),
	m_IndexRange(0, 0)
{

}

Menu::~Menu()
{

}

void Menu::AddMenuItem(const Texture &texture)
{
	m_MenuSprites.push_back(Sprite());
	m_MenuSprites.back().SetTexture(texture);

	m_IndexRange.y = m_MenuSprites.size() - 1;

	UpdateItemPositions();
	UpdateMaxSize();
}

void Menu::AlignVertical()
{
	for (int i = 0; i < m_MenuSprites.size(); i++)
	{
		m_MenuSprites[i].SetPosition(m_MenuSprites[i].GetPosition().x - m_MenuSprites[i].GetSize().x / 2, m_MenuSprites[i].GetPosition().y);
	}
}

void Menu::IncrementMenuIndex()
{
	SetSelectionIndex(m_MenuIndex + 1);
}

void Menu::DecrementMenuIndex()
{
	SetSelectionIndex(m_MenuIndex - 1);
}

void Menu::SetSelectionIndex(unsigned char newIndex)
{
	// Restore colour of previously selected menu item
	m_MenuSprites[m_MenuIndex].ClearPixelOverrideColour();

	if (newIndex < m_IndexRange.x)
		m_MenuIndex = m_IndexRange.x;
	else if (newIndex > m_IndexRange.y)
		m_MenuIndex = m_IndexRange.y;
	else
		m_MenuIndex = newIndex;

	// Highlight newly selected menu item
	m_MenuSprites[m_MenuIndex].SetPixelOverrideColour(m_SelectionColour);
}

void Menu::SetMinimumIndex(unsigned char index)
{
	if (index > m_MenuSprites.size())
		return;

	m_IndexRange.x = index;
	SetSelectionIndex(index);
}

void Menu::SetMaximumIndex(unsigned char index)
{
	if (index > m_MenuSprites.size())
		return;

	m_IndexRange.y = index;
	SetSelectionIndex(index);
}

void Menu::Render(ASCIIRenderer* pRenderer)
{
	for (Sprite& menuSprite : m_MenuSprites)
		menuSprite.Render(pRenderer);
}

void Menu::SetPosition(short x, short y)
{ 
	m_Position.x = x; 
	m_Position.y = y; 

	UpdateItemPositions();
}

void Menu::SetItemSpacing(char spacing)
{
	m_ItemSpacing = spacing;

	UpdateItemPositions();
	UpdateMaxSize();
}

void Menu::UpdateMaxSize()
{
	m_Size.y = 0;

	for (Sprite& item : m_MenuSprites)
	{
		if (item.GetSize().x > m_Size.x)
			m_Size.x = item.GetSize().x;

		m_Size.y += item.GetSize().y;
	}

	m_Size.y += (m_MenuSprites.size() - 1) * m_ItemSpacing;
}

void Menu::UpdateItemPositions()
{
	m_MenuSprites[0].SetPosition(m_Position.x + m_MenuSprites[0].GetPosition().x, m_Position.y + m_MenuSprites[0].GetPosition().y);

	for (int i = 1; i < m_MenuSprites.size(); i++)
	{
		m_MenuSprites[i].SetPosition(
			m_Position.x + m_MenuSprites[i].GetPosition().x, 
			m_MenuSprites[i - 1].GetPosition().y + m_MenuSprites[i - 1].GetSize().y + m_ItemSpacing);
	}
}