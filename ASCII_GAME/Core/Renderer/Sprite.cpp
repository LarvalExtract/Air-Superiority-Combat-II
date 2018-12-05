#include "Sprite.h"
#include "../Utils.h"
#include "ASCIIRenderer.h"

#include "../../TGAFile.h"

// Default path for sprites
const std::string SPRITE_FOLDER_PATH = "../sprites/";

const wchar_t FILL = L'\u2588';
const wchar_t SHADE_LIGHT = L'\u2591';
const wchar_t SHADE_MEDIUM = L'\u2592';
const wchar_t SHADE_DARK = L'\u2593';

// Function prototypes
CONSOLE_PIXEL SamplePixel(unsigned int pixel);
void ConvertPixels(unsigned int* source, CONSOLE_PIXEL* &destination, Vec2<int> size);

Sprite::Sprite() :
	m_PixelData(nullptr),
	m_bInitialised(false),
	m_PixelOverride({ 0, 0 })
{
}

Sprite::Sprite(const Sprite& other)
{
	m_Position = other.m_Position;
	m_Size = other.m_Size;
	m_bInitialised = other.m_bInitialised;
	m_PixelOverride = other.m_PixelOverride;

	SAFE_DELETE_ARY(m_PixelData);

	m_PixelData = new CONSOLE_PIXEL[m_Size.x * m_Size.y];
	std::memcpy(&m_PixelData[0], &other.m_PixelData[0], m_Size.x * m_Size.y);
}

Sprite::Sprite(const char* bmpFile) :
	m_PixelData(nullptr),
	m_bInitialised(false),
	m_PixelOverride({ 0, 0 })
{
	SetImage(bmpFile);
}

Sprite::~Sprite()
{
	SAFE_DELETE_ARY(m_PixelData);
}

void Sprite::SetImage(const char* bmpFile)
{
	SAFE_DELETE_ARY(m_PixelData);

	TGAFile tga(SPRITE_FOLDER_PATH + bmpFile);

	// Console pixels are half as wide as they are tall
	// Width is doubled to make square pixels
	m_Size.x = tga.Width() * 2;
	m_Size.y = tga.Height();

	ConvertPixels(tga.Pixels(), m_PixelData, m_Size);

	m_bInitialised = true;
}

void Sprite::Initialise(unsigned int* bgraPixels, Vec2<int>& size)
{
	m_Size.x = size.x * 2;
	m_Size.y = size.y;

	ConvertPixels(bgraPixels, m_PixelData, m_Size);

	m_bInitialised = true;
}

void ConvertPixels(unsigned int* source, CONSOLE_PIXEL* &destination, Vec2<int> size)
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

//void Sprite::Initialise(int* pixels, Vector2& size)
//{
//	if (m_bInitialised)
//		return;
//
//	m_Size = size;
//
//	m_PixelData = new CHAR_INFO*[m_Size.y];
//
//	for (int i = 0; i < m_Size.y; i++)
//	{
//		m_PixelData[i] = new CHAR_INFO[m_Size.x];
//	}
//
//	for (int y = 0; y < m_Size.y; y++)
//	{
//		for (int x = 0; x < m_Size.x; x++)
//		{
//			m_PixelData[y][x].Char.UnicodeChar = 0;
//			m_PixelData[y][x].Char.AsciiChar = 0;
//			m_PixelData[y][x].Attributes = pixels[x + (y*m_Size.x)];
//		}
//	}
//	m_bInitialised = true;
//}

//void Sprite::Initialise(CHAR_INFO* pixels, Vector2& size)
//{
//	if (m_bInitialised)
//		return;
//
//	m_Size = size;
//
//	m_PixelData = new CHAR_INFO*[m_Size.y];
//
//	for (int i = 0; i < m_Size.y; i++)
//	{
//		m_PixelData[i] = new CHAR_INFO[m_Size.x];
//	}
//
//	for (int y = 0; y < m_Size.y; y++)
//	{
//		for (int x = 0; x < m_Size.x; x++)
//		{
//			m_PixelData[y][x] = pixels[x + (y*m_Size.x)];
//		}
//	}
//
//	m_bInitialised = true;
//}

//void Sprite::Initialise(char* pixels, ConsoleColour colour, Vector2& size)
//{
//	if (m_bInitialised)
//		return;
//
//	m_Size = size;
//	m_Colour = colour;
//
//	m_PixelData = new CHAR_INFO*[m_Size.y];
//
//	for (int i = 0; i < m_Size.y; i++)
//	{
//		m_PixelData[i] = new CHAR_INFO[m_Size.x];
//	}
//
//	for (int y = 0; y < m_Size.y; y++)
//	{
//		for (int x = 0; x < m_Size.x; x++)
//		{
//			m_PixelData[y][x].Char.UnicodeChar = 0;
//			m_PixelData[y][x].Char.AsciiChar = pixels[x + (y*m_Size.x)];
//			m_PixelData[y][x].Attributes = static_cast<unsigned short>(m_Colour);
//		}
//	}
//	m_bInitialised = true;
//}

//void Sprite::Initialise(wchar_t* pixels, ConsoleColour colour, Vector2& size)
//{
//	if (m_bInitialised)
//		return;
//
//	m_Size = size;
//	m_Colour = colour;
//
//	m_PixelData = new CHAR_INFO*[m_Size.y];
//
//	for (int i = 0; i < m_Size.y; i++)
//	{
//		m_PixelData[i] = new CHAR_INFO[m_Size.x];
//	}
//
//	for (int y = 0; y < m_Size.y; y++)
//	{
//		for (int x = 0; x < m_Size.x; x++)
//		{
//			m_PixelData[y][x].Char.UnicodeChar = pixels[x + (y*m_Size.x)];
//			m_PixelData[y][x].Char.AsciiChar = pixels[x + (y*m_Size.x)];
//			m_PixelData[y][x].Attributes = static_cast<unsigned short>(m_Colour);
//		}
//	}
//	m_bInitialised = true;
//}

void Sprite::Render(ASCIIRenderer* pRenderer)
{
	if (!m_bInitialised)
		return;

	if (pRenderer)
	{
		for (int y = 0; y < m_Size.y; y++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				int index = x + (y * m_Size.x);

				if (m_PixelData[index].bOpaque)
				{
					if (m_PixelOverride.Attributes == 0)
						pRenderer->SetPixel(static_cast<int>(m_Position.x) + x, static_cast<int>(m_Position.y) + y, m_PixelData[index].char_info);
					else
						pRenderer->SetPixel(static_cast<int>(m_Position.x) + x, static_cast<int>(m_Position.y) + y, m_PixelOverride);
				}
			}
		}
	}
}

bool Sprite::Collides(Sprite& otherSprite)
{
	if (m_Position.x < (otherSprite.GetPosition().x + otherSprite.GetSize().x) &&
		(m_Position.x + m_Size.x) > otherSprite.GetPosition().x &&

		m_Position.y < (otherSprite.GetPosition().y + otherSprite.GetSize().y) &&
		(m_Position.y + m_Size.y) > otherSprite.GetPosition().y)
	{
		return true;
	}
	return false;
}

// Converts a 32-bit BGRA pixel to a console character
// Lots of colour accuracy lost!
CONSOLE_PIXEL SamplePixel(unsigned int pixel)
{
	// Extract BGRA channels to seperate unsigned chars
	unsigned char b = pixel & 0xFF;
	unsigned char g = (pixel >> 8) & 0xFF;
	unsigned char r = (pixel >> 16) & 0xFF;
	unsigned char a = (pixel >> 24) & 0xFF;

	// Reduce colour accuracy from 256 values to 4
	b >>= 6;
	g >>= 6;
	r >>= 6;
	a >>= 7;	// Alpha becomes 0 (transparent) or 1 (opaque)

	CONSOLE_PIXEL character;
	character.char_info.Char.UnicodeChar = 0;
	character.char_info.Attributes = 0;
	character.bOpaque = a;

	// Return character if it's transparent
	if (!character.bOpaque)
		return character;

	switch (r)
	{
	case 1: character.char_info.Attributes |= BACKGROUND_RED; break;
	case 2: character.char_info.Attributes |= BACKGROUND_RED; break;
	case 3: character.char_info.Attributes |= BACKGROUND_RED | BACKGROUND_INTENSITY; break;
	}

	switch (g)
	{
	case 1: character.char_info.Attributes |= BACKGROUND_GREEN; break;
	case 2: character.char_info.Attributes |= BACKGROUND_GREEN; break;
	case 3: character.char_info.Attributes |= BACKGROUND_GREEN | BACKGROUND_INTENSITY; break;
	}

	switch (b)
	{
	case 1: character.char_info.Attributes |= BACKGROUND_BLUE; break;
	case 2: character.char_info.Attributes |= BACKGROUND_BLUE; break;
	case 3: character.char_info.Attributes |= BACKGROUND_BLUE | BACKGROUND_INTENSITY; break;
	}

	return character;
}