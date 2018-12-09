#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "ConsoleColours.h"
#include "Core/Texture.h"

class ASCIIRenderer;

class Sprite
{
public:

	Sprite();
	~Sprite();

	void SetTexture(const Texture &texture) { m_Texture = texture; }

	//void Initialise(unsigned int* bgraPixels, Vec2<int>& size);
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
	Vec2<short> GetSize(){ return m_Texture.m_size; }

	bool Collides(Sprite& otherSprite);

protected:
	Vec2<float> m_Position;

	Texture m_Texture;

	CHAR_INFO m_PixelOverride;
};

#endif
