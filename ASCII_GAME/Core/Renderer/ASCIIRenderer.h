#ifndef _ASCII_RENDERER_H_
#define _ASCII_RENDERER_H_

#include "ConsoleColours.h"
#include <Windows.h>

enum ScreenSize : short
{
	SCREENSIZE_SMALL = 2,
	SCREENSIZE_LARGE = 4
};

class ASCIIRenderer
{
public:
	ASCIIRenderer();
	~ASCIIRenderer();

	void Initialise(int width, int height);
	void InitialisePixelSize();

	ScreenSize GetPixelSize() const { return m_ScreenSize; }
	void SetPixelSize(ScreenSize size);

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

	ScreenSize m_ScreenSize;

	bool m_bInitialised;
};

#endif
