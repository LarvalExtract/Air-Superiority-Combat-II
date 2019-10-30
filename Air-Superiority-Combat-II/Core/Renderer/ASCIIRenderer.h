#ifndef _ASCII_RENDERER_H_
#define _ASCII_RENDERER_H_

#include "ConsoleColours.h"
#include <Windows.h>
#include <string>

class ASCIIRenderer
{
public:
	ASCIIRenderer();
	~ASCIIRenderer();

	void Initialise(int width, int height, short fontWidth, short fontHeight);
	
	void SetTitle(const std::string &title);

	void TogglePixelSize();

	void Render();

	void SetPixel(int x, int y, CHAR_INFO& pPixelData);
	void ClearScreen(ConsoleColour colour = ConsoleColour::BACKGROUND_BLACK);

	short GetWidth() const { return m_ScreenSize.X; }
	short GetHeight() const { return m_ScreenSize.Y; }
	short GetFontWidth() const { return font.dwFontSize.X; }
	short GetFontHeight() const { return font.dwFontSize.Y; }

private:
	void SetPixelSize(short fontWidth, short fontHeight);
	void SetWindow(int width, int height);	

	HANDLE m_hConsole;
	CHAR_INFO* m_ScreenData;

	COORD m_ScreenSize;
	SMALL_RECT m_WriteRegion;
	CONSOLE_FONT_INFOEX font;

	ConsoleColour m_BackgroundColour;

	bool m_bInitialised;
};

#endif
