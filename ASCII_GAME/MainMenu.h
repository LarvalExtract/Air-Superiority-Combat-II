#pragma once

#include "Core/Renderer/Sprite.h"

class ASCIIRenderer;

const int MAX_MENU_ITEMS = 4;

enum MenuOption
{
	OPTION_START_GAME,
	OPTION_HIGH_SCORE,
	OPTION_QUIT_GAME
};

class MainMenu
{
public:
	MainMenu() : m_MenuIndex(1), m_bKeyIsPressed(false) {}

	void Initialise(int screenWidth);
	void Update();
	void Render(ASCIIRenderer* pRenderer);

	MenuOption GetSelectedMenuOption();

private:
	Sprite menuSprites[MAX_MENU_ITEMS];

	unsigned char m_MenuIndex;
	bool m_bKeyIsPressed;
};