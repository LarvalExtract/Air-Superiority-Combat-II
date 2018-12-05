#include "SpriteAnimation.h"
#include "TGAFile.h"
#include "Core/Utils.h"
#include "Core/Renderer/ASCIIRenderer.h"

SpriteAnimation::SpriteAnimation() :
	m_Frames(nullptr),
	m_FrameTime(0.0f),
	m_FrameCount(0),
	m_CurrentFrame(0),
	m_bActive(false),
	m_Position(0, 0),
	m_Size(0, 0)
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

	m_Size.x = file.Width() / frameCount;
	m_Size.y = file.Height();

	int rowLength = m_Size.x * sizeof(unsigned int);
	int frameSize = rowLength * m_Size.y;

	unsigned int* buffer = new unsigned int[m_Size.x * m_Size.y];
	unsigned int* tgaPixels = file.Pixels();

	for (int i = 0; i < m_FrameCount; i++)
	{
		for (int row = 0; row < m_Size.y; row++)
		{
			std::memcpy(&buffer[row * m_Size.x], &tgaPixels[(i * m_Size.x) + (row * m_Size.x * m_FrameCount)], rowLength);
		}

		m_Frames[i].Initialise(buffer, m_Size);
	}

	delete[] buffer;
}

void SpriteAnimation::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;

	if (m_Frames)
	{
		for (int i = 0; i < m_FrameCount; i++)
			m_Frames[i].SetPosition(m_Position);
	}
}

void SpriteAnimation::Update(float deltaTime)
{
	static float time = 0.0f;

	time += deltaTime;

	if (time > 0.05f)
	{
		m_CurrentFrame++;
		time = 0.0f;
	}

	if (m_CurrentFrame == m_FrameCount)
	{
		m_CurrentFrame = 0;
		m_bActive = false;
	}
}

void SpriteAnimation::Render(ASCIIRenderer *pRenderer)
{
	if (m_CurrentFrame >= 0 && m_CurrentFrame <= m_FrameCount)
		if (m_Frames)
			m_Frames[m_CurrentFrame].Render(pRenderer);
}