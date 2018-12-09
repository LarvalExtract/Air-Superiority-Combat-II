#pragma once

#include "Core/Renderer/Sprite.h"
#include "TextureArray.h"

class SpriteAnimation : public Sprite
{
public:
	SpriteAnimation();
	SpriteAnimation(const TextureArray &textureArray);
	~SpriteAnimation();

	void SetTextureArray(const TextureArray &textureArray) { m_TextureArray = textureArray; }
	void SetFrame(int frameNumber) { m_CurrentFrame = frameNumber; }
	void SetFrameTime(float frameTime) { m_FrameTime = frameTime; }

	int GetLoopCount() const { return m_LoopCount; }

	void Update(float deltaTime);

private:
	TextureArray m_TextureArray;
	float m_FrameTime;
	int m_CurrentFrame;
	unsigned int m_LoopCount;
};