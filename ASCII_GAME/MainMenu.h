#pragma once

#include "Core/Renderer/Sprite.h"

#include <vector>

class ASCIIRenderer;

class Menu
{
public:
	Menu();
	~Menu();

	void Render(ASCIIRenderer* pRenderer);

	void AddMenuItem(const Texture& texture);

	void IncrementMenuIndex();
	void DecrementMenuIndex();

	unsigned char GetSelectedMenuOption() const { return m_MenuIndex; }

	void AlignVertical();

	void SetPosition(short x, short y);
	void SetItemSpacing(char spacing);
	void SetSelectionColour(ConsoleColour colour) { m_SelectionColour = colour; }

	void SetSelectionIndex(unsigned char index);

	void SetMinimumIndex(unsigned char index) { m_IndexRange.x = index; }
	void SetMaximumIndex(unsigned char index) { m_IndexRange.y = index; }

private:
	std::vector<Sprite> m_MenuSprites;

	ConsoleColour m_SelectionColour;

	Vec2<short> m_Position;
	Vec2<short> m_Size;

	char m_ItemSpacing;
	unsigned char m_MenuIndex;
	Vec2<char> m_IndexRange;

	void UpdateMaxSize();
	void UpdateItemPositions();
};