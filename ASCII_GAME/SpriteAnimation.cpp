#include "SpriteAnimation.h"
#include "TGAFile.h"
#include "Core/Utils.h"
#include "Core/Renderer/ASCIIRenderer.h"

SpriteAnimation::SpriteAnimation() :
	m_Frames(nullptr),
	m_FrameTime(0.0f),
	m_FrameCount(0)
{

}

SpriteAnimation::~SpriteAnimation()
{
	SAFE_DELETE_ARY(m_Frames);
}

void SpriteAnimation::SetSpriteSheet(const char* tgaFilePath, int frameCount)
{
	SAFE_DELETE_ARY(m_Frames);

	TGAFile file(tgaFilePath);

	m_FrameCount = frameCount;
	m_Frames = new Sprite[m_FrameCount];

	Vec2<int> size;
	size.x = file.Width() / frameCount;
	size.y = file.Height();

	int rowLength = size.x * sizeof(unsigned int);
	int frameSize = rowLength * size.y;

	unsigned int* buffer = new unsigned int[size.x * size.y];
	unsigned int* tgaPixels = file.Pixels();

	for (int i = 0; i < m_FrameCount; i++)
	{
		for (int row = 0; row < size.y; row++)
		{
			std::memcpy(&buffer[row * size.x], &tgaPixels[(i * size.x) + (row * size.x * m_FrameCount)], rowLength);
		}

		m_Frames[i].Initialise(buffer, size);
	}

	delete[] buffer;
}

void SpriteAnimation::SetPosition(float x, float y)
{
	if (m_Frames)
	{
		for (int i = 0; i < m_FrameCount; i++)
			m_Frames[i].SetPosition(x, y);
	}
}

void SpriteAnimation::Render(ASCIIRenderer *pRenderer, int frame)
{
	if (frame >= 0 && frame <= m_FrameCount)
		if (m_Frames)
			m_Frames[frame].Render(pRenderer);
}