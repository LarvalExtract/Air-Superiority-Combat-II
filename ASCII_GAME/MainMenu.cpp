#include "MainMenu.h"
#include "Core/Renderer/ASCIIRenderer.h"

#include <Windows.h>

enum MenuSprites
{
	MENU_TITLE,
	MENU_START_GAME,
	MENU_HIGH_SCORE,
	MENU_QUIT_GAME
};

void MainMenu::Initialise(int screenWidth)
{
	menuSprites[MENU_TITLE].SetImage("menu/menu_title.tga");
	menuSprites[MENU_START_GAME].SetImage("menu/menu_startgame.tga");
	menuSprites[MENU_HIGH_SCORE].SetImage("menu/menu_highscore.tga");
	menuSprites[MENU_QUIT_GAME].SetImage("menu/menu_quit.tga");

	int halfScreenWidth = screenWidth / 2;
	menuSprites[MENU_TITLE].SetPosition(halfScreenWidth - menuSprites[MENU_TITLE].GetSize().x / 2, 50);
	menuSprites[MENU_START_GAME].SetPosition(halfScreenWidth - menuSprites[MENU_START_GAME].GetSize().x / 2, menuSprites[MENU_TITLE].GetPosition().y + menuSprites[MENU_TITLE].GetSize().y + 15);
	menuSprites[MENU_HIGH_SCORE].SetPosition(halfScreenWidth - menuSprites[MENU_HIGH_SCORE].GetSize().x / 2, menuSprites[MENU_START_GAME].GetPosition().y + menuSprites[MENU_START_GAME].GetSize().y + 5);
	menuSprites[MENU_QUIT_GAME].SetPosition(halfScreenWidth - menuSprites[MENU_QUIT_GAME].GetSize().x / 2, menuSprites[MENU_HIGH_SCORE].GetPosition().y + menuSprites[MENU_HIGH_SCORE].GetSize().y + 5);
}

void MainMenu::Update()
{
	if (GetKeyState(VK_UP) < 0)
	{
		if (!m_bKeyIsPressed)
		{
			menuSprites[m_MenuIndex].ClearPixelOverrideColour();

			if (m_MenuIndex > MENU_START_GAME)
				m_MenuIndex--;

			menuSprites[m_MenuIndex].SetPixelOverrideColour(ConsoleColour::BackgroundYellow);
		}

		m_bKeyIsPressed = true;
	}
	else if (GetKeyState(VK_DOWN) < 0)
	{
		if (!m_bKeyIsPressed)
		{
			menuSprites[m_MenuIndex].ClearPixelOverrideColour();

			if (m_MenuIndex < MAX_MENU_ITEMS - 1)
				m_MenuIndex++;

			menuSprites[m_MenuIndex].SetPixelOverrideColour(ConsoleColour::BackgroundYellow);
		}

		m_bKeyIsPressed = true;
	}
	else
	{
		m_bKeyIsPressed = false;
	}
}

MenuOption MainMenu::GetSelectedMenuOption()
{
	switch (m_MenuIndex)
	{
	case MENU_START_GAME: return OPTION_START_GAME;
	case MENU_HIGH_SCORE: return OPTION_HIGH_SCORE;
	case MENU_QUIT_GAME: return OPTION_QUIT_GAME;
	}
}

void MainMenu::Render(ASCIIRenderer* pRenderer)
{
	for (unsigned char i = 0; i < MAX_MENU_ITEMS; i++)
		menuSprites[i].Render(pRenderer);
}