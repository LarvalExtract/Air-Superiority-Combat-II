#include "Texture.h"
#include "Core/Utils.h"
#include "Core/Renderer/ConsoleColours.h"

// Function prototypes
void ConvertPixels(unsigned int*, CONSOLE_PIXEL* &, Vec2<short>);
CONSOLE_PIXEL SamplePixel(unsigned int);

std::vector<CONSOLE_PIXEL*> Texture::s_TextureCatalog;

Texture::Texture(const TGAFile &tgaFile) :
	m_pixelData(nullptr),
	m_size(0, 0)
{
	m_size.x = tgaFile.Width() * 2; // Double the width to ensure square proportions: console characters are half as wide as their height
	m_size.y = tgaFile.Height();

	ConvertPixels(tgaFile.Pixels(), m_pixelData, m_size);

	// Every time a texture is created, the pointer to its pixel data is stored in a static vector so that
	// all pixel data can be cleaned up at the end of the program
	Texture::s_TextureCatalog.push_back(m_pixelData);
}

Texture::Texture(CONSOLE_PIXEL* pixels, Vec2<short> size) : 
	m_pixelData(pixels), 
	m_size(size)
{
	Texture::s_TextureCatalog.push_back(m_pixelData);
}

Texture::~Texture()
{
	
}

void Texture::SetPixels(unsigned int* pixels, Vec2<short> size)
{
	m_size = size;
	m_size.x *= 2;

	ConvertPixels(pixels, m_pixelData, m_size);

	Texture::s_TextureCatalog.push_back(m_pixelData);
}

void Texture::CleanUpAllTextures()
{
	for (int i = 0; i < Texture::s_TextureCatalog.size(); i++)
	{
		if (Texture::s_TextureCatalog[i])
			delete[] s_TextureCatalog[i];
	}
}

void ConvertPixels(unsigned int* source, CONSOLE_PIXEL* &destination, Vec2<short> size)
{
	destination = new CONSOLE_PIXEL[size.x * size.y];

	int srcWidth = size.x / 2;
	for (int y = 0; y < size.y; y++)	// For each row of pixels in TGA file
	{
		for (int x = 0; x < size.x; x += 2)	// For each pixel in row
		{
			CONSOLE_PIXEL pixel = SamplePixel(source[(x >> 1) + (y * (srcWidth))]);

			int dstIndex = x + (y * size.x);

			destination[dstIndex] = pixel;		// Insert two pixels next to each other to create square pixels
			destination[dstIndex + 1] = pixel;
		}
	}
}

CONSOLE_PIXEL SamplePixel(unsigned int pixel)
{
	// Extract BGRA channels to seperate unsigned chars
	unsigned char b = pixel & 0xFF;
	unsigned char g = (pixel >> 8) & 0xFF;
	unsigned char r = (pixel >> 16) & 0xFF;
	unsigned char a = (pixel >> 24) & 0xFF;

	// Reduce colour accuracy from 256 values to 8
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

	if (r > 1) character.char_info.Attributes |= BACKGROUND_RED;	// Set red bit
	if (g > 1) character.char_info.Attributes |= BACKGROUND_GREEN;	// Set green bit
	if (b > 1) character.char_info.Attributes |= BACKGROUND_BLUE;	// Set blue bit

	if (brightness > 6) character.char_info.Attributes |= BACKGROUND_INTENSITY;	// Set intensity bit
	
	if (delta == 0 && brightness == 4)
		character.char_info.Attributes = BACKGROUND_DARK_GREY;	// Special case for dark grey colour


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