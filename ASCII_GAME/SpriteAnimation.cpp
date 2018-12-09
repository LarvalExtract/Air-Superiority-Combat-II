#include "SpriteAnimation.h"
#include "TGAFile.h"
#include "Core/Utils.h"
#include "Core/Renderer/ASCIIRenderer.h"

SpriteAnimation::SpriteAnimation() :
	m_FrameTime(0.0f),
	m_CurrentFrame(0),
	m_LoopCount(0)
{

}

SpriteAnimation::SpriteAnimation(const TextureArray &textureArray) :
	m_TextureArray(textureArray),
	m_FrameTime(0.05f),
	m_CurrentFrame(0),
	m_LoopCount(0)
{

}

SpriteAnimation::~SpriteAnimation()
{
	
}

void SpriteAnimation::Update(float deltaTime)
{
	static float time = 0.0f;

	time += deltaTime;

	if (time > m_FrameTime)
	{
		m_CurrentFrame++;

		if (m_CurrentFrame >= m_TextureArray.count)
		{
			m_CurrentFrame = 0;
			m_LoopCount++;
		}

		m_Texture = m_TextureArray.textures[m_CurrentFrame];
		time = 0.0f;
	}
}