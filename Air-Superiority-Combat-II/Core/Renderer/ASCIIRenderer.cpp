#include "ASCIIRenderer.h"
#include "Core/Utils.h"

ASCIIRenderer::ASCIIRenderer() :
	m_ScreenData(NULL),
	m_Width(0),
	m_Height(0),
	m_bInitialised(false),
	m_BackgroundColour(ConsoleColour::BACKGROUND_BLACK)
{
}

ASCIIRenderer::~ASCIIRenderer()
{
	SAFE_DELETE_ARY(m_ScreenData);
}

void ASCIIRenderer::Initialise(int width, int height)
{
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	InitialisePixelSize();
	
	SetWindow(width, height);
	
	m_ScreenData = new CHAR_INFO[m_Width * m_Height];
	
	ClearScreen(ConsoleColour::BACKGROUND_SKYBLUE);

	m_bInitialised = true;
}

void ASCIIRenderer::InitialisePixelSize()
{
	//---Set up font size to look like pixel---
	font_size.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(m_hConsole, false, &font_size);

	font_size.dwFontSize.X = 1;	//Width of element in buffer
	font_size.dwFontSize.Y = 4;	//Hieght of element in buffer

	SetCurrentConsoleFontEx(m_hConsole, false, &font_size);	//Set the new font size
}

void ASCIIRenderer::TogglePixelSize()
{
	font_size.dwFontSize.Y == 2 ? font_size.dwFontSize.Y = 4 : font_size.dwFontSize.Y = 2;

	SetCurrentConsoleFontEx(m_hConsole, false, &font_size);
}

void ASCIIRenderer::SetWindow(int Width, int Height)
{
	COORD MaxWindowSize = GetLargestConsoleWindowSize(m_hConsole);

	m_Width = min(MaxWindowSize.X, Width);
	m_Height = min(MaxWindowSize.Y, Height);
	
	SMALL_RECT rect = { 0, 0, m_Width - 1, m_Height - 1 };
	COORD coord = { m_Width, m_Height };

	bool bufferSizeSet = SetConsoleScreenBufferSize(m_hConsole, coord);
	bool windowInfoSet = SetConsoleWindowInfo(m_hConsole, TRUE, &rect);

	LPCTSTR windowTitle = L"Air Superiority Combat II (4107COMP Assignment 1)";
	SetConsoleTitle(windowTitle);
}

void ASCIIRenderer::ClearScreen(ConsoleColour colour)
{
	int numPixels = (m_Width * m_Height);
	for (int i = 0; i < numPixels; i++)
	{
		m_ScreenData[i].Char.UnicodeChar = 0;
		m_ScreenData[i].Char.AsciiChar = 0; // initialise screen to 0 as we create it
		m_ScreenData[i].Attributes = static_cast<unsigned short>(colour);
	}
}

void ASCIIRenderer::SetPixel(int x, int y, CHAR_INFO& pPixelData)
{
	if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
	{
		m_ScreenData[x + (y * m_Width)] = pPixelData;
	}
}

void ASCIIRenderer::Render()
{
	static COORD coord = { m_Width, m_Height };
	static COORD coord2 = { 0, 0 };
	static SMALL_RECT write = { 0, 0, m_Width-1, m_Height-1 };			// Region to write to

	WriteConsoleOutput(m_hConsole, (const CHAR_INFO*)(m_ScreenData), coord, coord2, &write);
}
