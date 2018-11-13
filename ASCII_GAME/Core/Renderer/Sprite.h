#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "../Maths/Vector2.h"
#include "ConsoleColours.h"

#include <Windows.h>

class ASCIIRenderer;

// Custom character which adds a transparency flag
struct CONSOLE_PIXEL
{
	CHAR_INFO charInfo;
	bool transparent;
};

class Sprite
{
public:

	Sprite();
	Sprite(const char* bmpFile);
	~Sprite();

	void SetImage(const char* bmpFile);

	//void Initialise(int* pixels, Vector2& size);
	//void Initialise(CHAR_INFO* pixels, Vector2& size);
	//void Initialise(char* pixels, ConsoleColour colour, Vector2& size);
	//void Initialise(wchar_t* pixels, ConsoleColour colour, Vector2& size);

	void Render(ASCIIRenderer* pRenderer);

	void SetPosition(int x, int y) { m_Position.x = x; m_Position.y = y; }
	void SetPosition(Vector2& position) { m_Position = position; }

	Vector2 GetPosition(){ return m_Position; }
	Vector2 GetSize(){ return m_Size; }

	bool Collides(Sprite& otherSprite);

protected:
	Vector2 m_Position;
	Vector2 m_Size;

private:
	bool m_bInitialised;

	CONSOLE_PIXEL* m_PixelData;
};

#endif
