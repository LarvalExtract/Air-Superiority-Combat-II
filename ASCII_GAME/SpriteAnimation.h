#pragma once

#include "Core/Renderer/Sprite.h"

class SpriteAnimation
{
public:
	SpriteAnimation();
	~SpriteAnimation();

	Vec2<float> GetPosition() const { return m_Position; }
	Vec2<int> GetSize() const { return m_Size; }

	void SetSpriteSheet(const char* tgaFilePath, int frameCount);
	void SetFrame(int frameNumber) { m_CurrentFrame = frameNumber; }
	void SetFrameTime(float frameTime) { m_FrameTime = frameTime; }
	void SetPosition(float x, float y);

	bool Active() const { return m_bActive; }
	void SetActive(bool active) { m_bActive = active; }

	void Update(float deltaTime);

	void Render(ASCIIRenderer *pRenderer);

private:
	Sprite* m_Frames;
	float m_FrameTime;
	int m_FrameCount;
	int m_CurrentFrame;
	bool m_bActive;

	Vec2<float> m_Position;
	Vec2<int> m_Size;
};