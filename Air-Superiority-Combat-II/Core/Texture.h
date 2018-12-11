#pragma once

#include <Windows.h>

#include "Core/Maths/Vec2.h"
#include "Core/TGAFile.h"

#include <vector>

struct CONSOLE_PIXEL
{
	CHAR_INFO char_info;
	bool bOpaque;
};

class Texture
{
public:
	Texture() : m_pixelData(nullptr), m_size(0, 0) {}
	Texture(const TGAFile& tgaFile);
	Texture(CONSOLE_PIXEL* pixels, Vec2<short> size) : m_pixelData(pixels), m_size(size) {}
	~Texture();

	void SetPixels(unsigned int* pixels, Vec2<short> size);

	static void ConvertPixels(unsigned int*, CONSOLE_PIXEL* &, Vec2<short>);
	static CONSOLE_PIXEL SamplePixel(unsigned int);

	CONSOLE_PIXEL* m_pixelData;
	Vec2<short> m_size;
};