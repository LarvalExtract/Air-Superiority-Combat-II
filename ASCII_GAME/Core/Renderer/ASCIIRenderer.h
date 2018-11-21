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
	void SetPixelSize(unsigned char size);

	void Render();

	void SetPixel(int x, int y, CHAR_INFO& pPixelData);
	void ClearScreen(ConsoleColour colour = ConsoleColour::BackgroundCyan);	

private:

	void SetWindow(int width, int height);
	

	HANDLE m_hConsole;
	CHAR_INFO* m_ScreenData;
	short m_Width;
	short m_Height;
	ConsoleColour m_BackgroundColour;

	bool m_bInitialised;
};

#endif
