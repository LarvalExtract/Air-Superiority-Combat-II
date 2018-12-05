#pragma once

#include "Core/Renderer/Sprite.h"
#include <string>

class SpriteCharacter : public Sprite
{
public:
	SpriteCharacter(const std::string& path, unsigned char leading, unsigned char tracking, unsigned char kerning) :
		m_Lead(leading),
		m_Track(tracking),
		m_Kern(kerning) 
	{
		SetImage(path.c_str());
	}

	void SetLeading(unsigned char lead) { m_Lead = lead; }
	void SetTracking(unsigned char track) { m_Track = track; }
	void SetKerning(unsigned char kern) { m_Kern = kern; }

	unsigned char GetLeading() const { return m_Lead; }
	unsigned char GetTracking() const { return m_Track; }
	unsigned char GetKerning() const { return m_Kern; }

private:
	unsigned char m_Lead;
	unsigned char m_Track;
	unsigned char m_Kern;
};