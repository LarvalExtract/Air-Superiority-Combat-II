#include "TextureArray.h"

TextureArray::TextureArray(const const TGAFile &spriteSheet, unsigned int frameCount) :
	textures(nullptr),
	count(frameCount)
{
	m_Size.x = spriteSheet.Width() / count;
	m_Size.y = spriteSheet.Height();

	textures = new Texture[count];

	unsigned int* buffer = new unsigned int[m_Size.x * m_Size.y];
	unsigned int* tgaPixels = spriteSheet.Pixels();

	int rowLength = m_Size.x * 4;
	for (int i = 0; i < count; i++)
	{
		for (int row = 0; row < m_Size.y; row++)
		{
			std::memcpy(&buffer[row * m_Size.x], &tgaPixels[(i * m_Size.x) + (row * m_Size.x * count)], rowLength);
		}

		textures[i].SetPixels(buffer, m_Size);
	}

	delete[] buffer;
}

TextureArray::~TextureArray()
{

}