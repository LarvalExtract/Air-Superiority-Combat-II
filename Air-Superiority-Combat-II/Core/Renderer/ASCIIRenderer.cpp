#include "ASCIIRenderer.h"
#include "Core/Utils.h"

ASCIIRenderer::ASCIIRenderer() :
	m_ScreenData(NULL),
	m_ScreenSize({ 0, 0 }),
	m_bInitialised(false),
	m_BackgroundColour(ConsoleColour::BACKGROUND_BLACK)
{
}

ASCIIRenderer::~ASCIIRenderer()
{
	SAFE_DELETE_ARY(m_ScreenData);
}

void ASCIIRenderer::Initialise(int width, int height, short fontWidth, short fontHeight)
{
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetPixelSize(fontWidth, fontHeight);	
	SetWindow(width, height);
	
	m_ScreenData = new CHAR_INFO[m_ScreenSize.X * m_ScreenSize.Y];
	
	ClearScreen(ConsoleColour::BACKGROUND_SKYBLUE);

	SetConsoleActiveScreenBuffer(m_hConsole);

	m_bInitialised = true;
}

void ASCIIRenderer::SetPixelSize(short fontWidth, short fontHeight)
{
	//---Set up font size to look like pixel---
	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(m_hConsole, false, &font);

	wcscpy_s(font.FaceName, L"Terminal");
	font.dwFontSize.X = fontWidth;	//Width of element in buffer
	font.dwFontSize.Y = fontHeight;	//Hieght of element in buffer

	SetCurrentConsoleFontEx(m_hConsole, false, &font);	//Set the new font size
	GetCurrentConsoleFontEx(m_hConsole, false, &font);
}

void ASCIIRenderer::SetTitle(const std::string &title)
{
	SetConsoleTitle(title.c_str());
}

void ASCIIRenderer::TogglePixelSize()
{
	font.dwFontSize.Y == 2 ? font.dwFontSize.Y = 4 : font.dwFontSize.Y = 4;

	SetCurrentConsoleFontEx(m_hConsole, false, &font);
}

void ASCIIRenderer::SetWindow(int Width, int Height)
{
	COORD MaxWindowSize = GetLargestConsoleWindowSize(m_hConsole);

	m_ScreenSize.X = Width < MaxWindowSize.X ? Width : MaxWindowSize.X;
	m_ScreenSize.Y = Height < MaxWindowSize.Y ? Height : MaxWindowSize.Y;
	
	m_WriteRegion = { 0, 0, m_ScreenSize.X - 1, m_ScreenSize.Y - 1 };

	bool bufferSizeSet = SetConsoleScreenBufferSize(m_hConsole, m_ScreenSize);
	bool windowInfoSet = SetConsoleWindowInfo(m_hConsole, TRUE, &m_WriteRegion);
}

void ASCIIRenderer::ClearScreen(ConsoleColour colour)
{
	int numPixels = (m_ScreenSize.X * m_ScreenSize.Y);
	for (int i = 0; i < numPixels; i++)
	{
		m_ScreenData[i].Char.UnicodeChar = 0;
		m_ScreenData[i].Char.AsciiChar = 0; // initialise screen to 0 as we create it
		m_ScreenData[i].Attributes = static_cast<unsigned short>(colour);
	}
}

void ASCIIRenderer::SetPixel(int x, int y, CHAR_INFO& pPixelData)
{
	if (x >= 0 && x < m_ScreenSize.X && y >= 0 && y < m_ScreenSize.Y)
	{
		m_ScreenData[x + (y * m_ScreenSize.X)] = pPixelData;
	}
}

void ASCIIRenderer::Render()
{
	static COORD coord2 = { 0, 0 };

	WriteConsoleOutput(m_hConsole, m_ScreenData, m_ScreenSize, coord2, &m_WriteRegion);
}
