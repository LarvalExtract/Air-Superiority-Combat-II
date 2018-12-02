#pragma once

#include "Core/Renderer/Sprite.h"

class SpriteAnimation
{
public:
	SpriteAnimation();
	~SpriteAnimation();

	void SetSpriteSheet(const char* tgaFilePath, int frameCount);
	void SetFrameTime(float frameTime) { m_FrameTime = frameTime; }
	void SetPosition(float x, float y);

	void Render(ASCIIRenderer *pRenderer, int frame);

private:
	Sprite* m_Frames;
	float m_FrameTime;
	int m_FrameCount;
};