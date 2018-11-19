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

Sprite::Sprite() :
	m_PixelData(nullptr),
	m_bInitialised(false),
	m_PixelOverride({ 0, 0 })
{
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

	m_Size.x = tga.Width() * 2;
	m_Size.y = tga.Height();

	m_PixelData = new CONSOLE_PIXEL[m_Size.x * m_Size.y];

	// TGA rows are stored bottom to top, must be copied in reverse order
	for (int y = 0; y < m_Size.y; y++)
	{
		for (int x = 0; x < m_Size.x; x += 2)
		{
			CONSOLE_PIXEL pixel = SamplePixel(tga.GetPixel((x >> 1) + (y * (tga.Width()))));

			int dstIndex = x + (y * m_Size.x);

			m_PixelData[dstIndex] = pixel;
			m_PixelData[dstIndex + 1] = pixel;
		}
	}

	m_bInitialised = true;
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

				if (!m_PixelData[index].transparent)
				{
					if (m_PixelOverride.Attributes != 0)
						pRenderer->SetPixel(static_cast<int>(m_Position.x) + x, static_cast<int>(m_Position.y) + y, m_PixelOverride);
					else
						pRenderer->SetPixel(static_cast<int>(m_Position.x) + x, static_cast<int>(m_Position.y) + y, m_PixelData[index].charInfo);
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
	unsigned char blue = (pixel & 0xFF) >> 6;
	unsigned char green = ((pixel >> 8) & 0xFF) >> 6;
	unsigned char red = ((pixel >> 16) & 0xFF) >> 6;
	unsigned char alpha = ((pixel >> 24) & 0xFF) >> 7;

	CONSOLE_PIXEL character;
	character.charInfo.Char.UnicodeChar = 0;
	character.charInfo.Attributes = 0;
	character.transparent = false;

	if (alpha == 0)
		character.transparent = true;

	else
	{
		switch (red)
		{
		case 1: character.charInfo.Attributes |= BACKGROUND_RED; break;
		case 2: character.charInfo.Attributes |= BACKGROUND_RED | BACKGROUND_INTENSITY; break;
		case 3: character.charInfo.Attributes |= BACKGROUND_RED | BACKGROUND_INTENSITY; break;
		}

		switch (green)
		{
		case 1: character.charInfo.Attributes |= BACKGROUND_GREEN; break;
		case 2: character.charInfo.Attributes |= BACKGROUND_GREEN | BACKGROUND_INTENSITY; break;
		case 3: character.charInfo.Attributes |= BACKGROUND_GREEN | BACKGROUND_INTENSITY; break;
		}

		switch (blue)
		{
		case 1: character.charInfo.Attributes |= BACKGROUND_BLUE; break;
		case 2: character.charInfo.Attributes |= BACKGROUND_BLUE | BACKGROUND_INTENSITY; break;
		case 3: character.charInfo.Attributes |= BACKGROUND_BLUE | BACKGROUND_INTENSITY; break;
		}
	}

	return character;
}