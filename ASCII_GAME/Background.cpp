#include "Background.h"

#include <chrono>
#include <cstdlib>

unsigned int Random(int min, int max);

Background::Background(int width, int height) :
	m_Boundary(width, height)
{
	srand(std::chrono::system_clock::time_point().time_since_epoch().count());

	clouds.reserve(10);
	for (int i = 0; i < 10; i++)
		clouds.emplace_back("cloud1.tga", Random(30, 60));

	for (BackgroundItem& sprite : clouds)
	{
		sprite.SetPosition(Random(0, m_Boundary.x), Random(0, m_Boundary.y - sprite.GetSize().y));
	}
}

Background::~Background()
{

}

void Background::Update(float deltaTime)
{
	for (BackgroundItem& sprite : clouds)
	{
		sprite.SetPosition(sprite.GetPosition().x - (sprite.GetSpeed() * deltaTime), sprite.GetPosition().y);

		// Sprite has escaped the left side of the screen
		if (sprite.GetPosition().x + sprite.GetSize().x < 0)
		{
			sprite.SetPosition(m_Boundary.x, Random(0, m_Boundary.y - sprite.GetSize().y));
		}
	}
}

void Background::Render(ASCIIRenderer* pRenderer)
{
	for (BackgroundItem& sprite : clouds)
		sprite.Render(pRenderer);
}

unsigned int Random(int min, int max)
{
	return rand() % (max - min + 1) + min;
}