#pragma once

#include "Core/Renderer/Sprite.h"
#include "Game/GameStates.h"

class ASCIIRenderer;

const int MAX_MENU_ITEMS = 5;

enum MenuOption : unsigned char
{
	OPTION_START_GAME = 1,
	OPTION_HIGH_SCORE,
	OPTION_CHANGE_SCREEN_SIZE,
	OPTION_QUIT_GAME
};

class MainMenu
{
public:
	MainMenu() : m_MenuIndex(1), m_bKeyIsPressed(false) {}

	void Initialise(int screenWidth);
	void Update();
	void Render(ASCIIRenderer* pRenderer);

	MenuOption GetSelectedMenuOption() const { return static_cast<MenuOption>(m_MenuIndex); }

private:
	Sprite menuSprites[MAX_MENU_ITEMS];

	unsigned char m_MenuIndex;
	bool m_bKeyIsPressed;
};