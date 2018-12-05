#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "../../Vec2.h"
#include "ConsoleColours.h"

#include <Windows.h>

class ASCIIRenderer;

// Custom character which adds a transparency flag
struct CONSOLE_PIXEL
{
	CHAR_INFO char_info;
	bool bOpaque;
};

class Sprite
{
public:

	Sprite();
	Sprite(const Sprite& other);
	Sprite(const char* bmpFile);
	~Sprite();

	void SetImage(const char* bmpFile);

	void Initialise(unsigned int* bgraPixels, Vec2<int>& size);
	//void Initialise(int* pixels, Vector2& size);
	//void Initialise(CHAR_INFO* pixels, Vector2& size);
	//void Initialise(char* pixels, ConsoleColour colour, Vector2& size);
	//void Initialise(wchar_t* pixels, ConsoleColour colour, Vector2& size);

	void Render(ASCIIRenderer* pRenderer);

	void SetPosition(float x, float y) { m_Position.x = x; m_Position.y = y; }
	void SetPosition(Vec2<float>& position) { m_Position = position; }
	void SetPixelOverrideColour(ConsoleColour colour) { m_PixelOverride.Attributes = colour; }
	void ClearPixelOverrideColour() { m_PixelOverride.Attributes = 0; }

	Vec2<float> GetPosition(){ return m_Position; }
	Vec2<int> GetSize(){ return m_Size; }

	bool Collides(Sprite& otherSprite);

protected:
	Vec2<float> m_Position;
	Vec2<int> m_Size;

protected:
	bool m_bInitialised;

	CONSOLE_PIXEL* m_PixelData;
	CHAR_INFO m_PixelOverride;
};

#endif
