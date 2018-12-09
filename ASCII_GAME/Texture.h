#pragma once

#include <Windows.h>

#include "Vec2.h"
#include "TGAFile.h"

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
	~Texture();

	void SetPixels(unsigned int* pixels, Vec2<short> size);

	CONSOLE_PIXEL* m_pixelData;
	Vec2<short> m_size;
};