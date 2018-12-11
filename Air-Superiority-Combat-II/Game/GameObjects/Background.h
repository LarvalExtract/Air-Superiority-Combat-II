#pragma once

#include "Core/Renderer/Sprite.h"
#include <vector>

class BackgroundItem : public Sprite
{
public:
	BackgroundItem() {}

	float GetSpeed() const { return m_Speed; }
	void SetSpeed(float speed) { m_Speed = speed; }

private:
	float m_Speed;
};

class Background
{
public:
	Background(int width, int height);
	~Background();

	void Update(float deltaTime);
	void Render(ASCIIRenderer* pRenderer);

private:
	std::vector<BackgroundItem> m_BackgroundItems;
	Vec2<int> m_Boundary;
};