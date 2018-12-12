#include "Sprite.h"
#include "../Utils.h"
#include "ASCIIRenderer.h"


// Default path for sprites
//const std::string SPRITE_FOLDER_PATH = "../sprites/";

//const wchar_t FILL = L'\u2588';
//const wchar_t SHADE_LIGHT = L'\u2591';
//const wchar_t SHADE_MEDIUM = L'\u2592';
//const wchar_t SHADE_DARK = L'\u2593';

Sprite::Sprite() :
	m_PixelOverride({ 0, 0 })
{

}

Sprite::~Sprite()
{
	
}

//void Sprite::Initialise(unsigned int* bgraPixels, Vec2<int>& size)
//{
//	m_Texture.size.x = size.x * 2;
//	m_Texture.size.y = size.y;
//
//	ConvertPixels(bgraPixels, m_Texture.pixelData, m_Texture.size);
//
//	m_bInitialised = true;
//}



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

void Sprite::Initialise(CONSOLE_PIXEL* pixels, Vec2<short> &size)
{
	m_Texture.m_size = size;

	m_Texture.m_pixelData = new CONSOLE_PIXEL[m_Texture.m_size.x * m_Texture.m_size.y];

	for (int y = 0; y < m_Texture.m_size.y; y++)
	{
		for (int x = 0; x < m_Texture.m_size.x; x++)
		{
			m_Texture.m_pixelData[x + (y*m_Texture.m_size.x)] = pixels[x + (y*m_Texture.m_size.x)];
		}
	}
}

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
	if (pRenderer)
	{
		for (int y = 0; y < m_Texture.m_size.y; y++)
		{
			for (int x = 0; x < m_Texture.m_size.x; x++)
			{
				int index = x + (y * m_Texture.m_size.x);

				if (m_Texture.m_pixelData[index].bOpaque)
				{
					if (m_PixelOverride.Attributes == 0)
						pRenderer->SetPixel(static_cast<int>(m_Position.x) + x, static_cast<int>(m_Position.y) + y, m_Texture.m_pixelData[index].char_info);
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
		(m_Position.x + m_Texture.m_size.x) > otherSprite.GetPosition().x &&

		m_Position.y < (otherSprite.GetPosition().y + otherSprite.GetSize().y) &&
		(m_Position.y + m_Texture.m_size.y) > otherSprite.GetPosition().y)
	{
		return true;
	}
	return false;
}