#include "Texture.h"
#include "Core/Utils.h"
#include "Core/Renderer/ConsoleColours.h"

Texture::Texture(const TGAFile &tgaFile) :
	m_pixelData(nullptr),
	m_size(0, 0)
{
	m_size.x = tgaFile.Width() * 2;
	m_size.y = tgaFile.Height();

	ConvertPixels(tgaFile.Pixels(), m_pixelData, m_size);
}

Texture::~Texture()
{

}

void Texture::SetPixels(unsigned int* pixels, Vec2<short> size)
{
	m_size = size;
	m_size.x *= 2;

	if (m_size.x == 24701)
		int dl;

	ConvertPixels(pixels, m_pixelData, m_size);
}

void Texture::ConvertPixels(unsigned int* source, CONSOLE_PIXEL* &destination, Vec2<short> size)
{
	destination = new CONSOLE_PIXEL[size.x * size.y];

	int srcWidth = size.x / 2;
	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x += 2)
		{
			CONSOLE_PIXEL pixel = SamplePixel(source[(x >> 1) + (y * (srcWidth))]);

			int dstIndex = x + (y * size.x);

			destination[dstIndex] = pixel;
			destination[dstIndex + 1] = pixel;
		}
	}
}

CONSOLE_PIXEL Texture::SamplePixel(unsigned int pixel)
{
	// Extract BGRA channels to seperate unsigned chars
	unsigned char b = pixel & 0xFF;
	unsigned char g = (pixel >> 8) & 0xFF;
	unsigned char r = (pixel >> 16) & 0xFF;
	unsigned char a = (pixel >> 24) & 0xFF;

	// Reduce colour accuracy from 256 values to 4
	r = static_cast<int>(r + 16) >> 5;
	g = static_cast<int>(g + 16) >> 5;
	b = static_cast<int>(b + 16) >> 5;
	a >>= 7;	// Alpha becomes 0 (transparent) or 1 (opaque)

	CONSOLE_PIXEL character;
	character.char_info.Char.UnicodeChar = 0;
	character.char_info.Attributes = 0;
	character.bOpaque = a;

	// Return character if it's transparent
	if (!character.bOpaque)
		return character;

	int brightness = Max(r, Max(g, b));
	int delta = brightness - Min(r, Min(g, b));

	if (r > 1) character.char_info.Attributes |= BACKGROUND_RED;
	if (g > 1) character.char_info.Attributes |= BACKGROUND_GREEN;
	if (b > 1) character.char_info.Attributes |= BACKGROUND_BLUE;

	if (brightness > 6) character.char_info.Attributes |= BACKGROUND_INTENSITY;
	
	if (delta == 0 && brightness == 4)
		character.char_info.Attributes = BACKGROUND_DARK_GREY;


	//switch (r)
	//{
	//case 1: character.char_info.Attributes |= BACKGROUND_RED; break;
	//case 2: character.char_info.Attributes |= BACKGROUND_RED; break;
	//case 3: character.char_info.Attributes |= BACKGROUND_RED | BACKGROUND_INTENSITY; break;
	//case 4: character.char_info.Attributes |= BACKGROUND_RED | BACKGROUND_INTENSITY; break;
	//}
	//
	//switch (g)
	//{
	//case 1: character.char_info.Attributes |= BACKGROUND_GREEN; break;
	//case 2: character.char_info.Attributes |= BACKGROUND_GREEN; break;
	//case 3: character.char_info.Attributes |= BACKGROUND_GREEN | BACKGROUND_INTENSITY; break;
	//case 4: character.char_info.Attributes |= BACKGROUND_GREEN | BACKGROUND_INTENSITY; break;
	//}
	//
	//switch (b)
	//{
	//case 1: character.char_info.Attributes |= BACKGROUND_BLUE; break;
	//case 2: character.char_info.Attributes |= BACKGROUND_BLUE; break;
	//case 3: character.char_info.Attributes |= BACKGROUND_BLUE | BACKGROUND_INTENSITY; break;
	//case 4: character.char_info.Attributes |= BACKGROUND_BLUE | BACKGROUND_INTENSITY; break;
	//}

	return character;
}