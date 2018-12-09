#ifndef _ASCII_RENDERER_H_
#define _ASCII_RENDERER_H_

#include "ConsoleColours.h"
#include <Windows.h>

class ASCIIRenderer
{
public:
	ASCIIRenderer();
	~ASCIIRenderer();

	void Initialise(int width, int height);
	void InitialisePixelSize();

	void TogglePixelSize();

	void Render();

	void SetPixel(int x, int y, CHAR_INFO& pPixelData);
	void ClearScreen(ConsoleColour colour);	

private:

	void SetWindow(int width, int height);
	

	HANDLE m_hConsole;
	CHAR_INFO* m_ScreenData;
	short m_Width;
	short m_Height;
	ConsoleColour m_BackgroundColour;
	PCONSOLE_FONT_INFOEX font_size;

	bool m_bInitialised;
};

#endif
