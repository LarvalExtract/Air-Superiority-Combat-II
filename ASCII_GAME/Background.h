#pragma once

#include "Core/Renderer/Sprite.h"
#include <vector>

class BackgroundItem : public Sprite
{
public:
	BackgroundItem(const char* file, float speed) : Sprite(file), m_Speed(speed) {}

	float GetSpeed() const { return m_Speed; }

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
	std::vector<BackgroundItem> clouds;

	Vec2<int> m_Boundary;
};