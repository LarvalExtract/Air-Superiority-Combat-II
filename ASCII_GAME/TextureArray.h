#pragma once

#include "Texture.h"

class TextureArray
{
public:
	TextureArray() : textures(nullptr), count(0) {}
	TextureArray(const TGAFile &spriteSheet, unsigned int frameCount);
	~TextureArray();

	Texture* textures;
	unsigned int count;
	Vec2<short> m_Size;
};