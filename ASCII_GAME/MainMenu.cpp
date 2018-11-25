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

void MainMenu::Initialise(int screenWidth)
{
	menuSprites[MENU_TITLE].SetImage("menu/menu_title.tga");
	menuSprites[MENU_START_GAME].SetImage("menu/menu_startgame.tga");
	menuSprites[MENU_HIGH_SCORE].SetImage("menu/menu_highscore.tga");
	menuSprites[MENU_SCREEN_SIZE].SetImage("menu/menu_screensize.tga");
	menuSprites[MENU_QUIT_GAME].SetImage("menu/menu_quit.tga");

	int halfScreenWidth = screenWidth / 2;

	menuSprites[MENU_TITLE].SetPosition(halfScreenWidth - menuSprites[MENU_TITLE].GetSize().x / 2, 40);

	for (unsigned char i = MENU_START_GAME; i < MAX_MENU_ITEMS; i++)
		menuSprites[i].SetPosition(halfScreenWidth - menuSprites[i].GetSize().x / 2, menuSprites[i - 1].GetPosition().y + menuSprites[i - 1].GetSize().y + 10);
	
	menuSprites[MENU_START_GAME].SetPixelOverrideColour(ConsoleColour::BACKGROUND_YELLOW);
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

			menuSprites[m_MenuIndex].SetPixelOverrideColour(ConsoleColour::BACKGROUND_YELLOW);
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

			menuSprites[m_MenuIndex].SetPixelOverrideColour(ConsoleColour::BACKGROUND_YELLOW);
		}

		m_bKeyIsPressed = true;
	}
	else
	{
		m_bKeyIsPressed = false;
	}
}

void MainMenu::Render(ASCIIRenderer* pRenderer)
{
	for (unsigned char i = 0; i < MAX_MENU_ITEMS; i++)
		menuSprites[i].Render(pRenderer);
}