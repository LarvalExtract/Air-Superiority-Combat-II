#include <Windows.h>

CHAR_INFO* screenBuffer;

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//SetConsoleActiveScreenBuffer(hConsole);

	CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(font);

	GetCurrentConsoleFontEx(hConsole, false, &font);

	wcscpy_s(font.FaceName, L"Terminal");
	font.dwFontSize.X = 8;
	font.dwFontSize.Y = 8;

	SetCurrentConsoleFontEx(hConsole, false, &font);

	COORD screenSize = { 100, 100 };
	SetConsoleScreenBufferSize(hConsole, screenSize);

	SMALL_RECT region = { 0, 0, screenSize.X - 1, screenSize.Y - 1 };
	SetConsoleWindowInfo(hConsole, true, &region);

	screenBuffer = new CHAR_INFO[screenSize.X * screenSize.Y];
	memset(screenBuffer, 0, screenSize.X * screenSize.Y);

	while (true)
	{


		WriteConsoleOutput(hConsole, screenBuffer, screenSize, { 0, 0 }, &region);
	}

	return 0;
}