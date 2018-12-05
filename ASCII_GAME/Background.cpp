#include "Background.h"
#include "Core/Utils.h"

#include <chrono>

Background::Background(int width, int height) :
	m_Boundary(width, height)
{
	srand(std::chrono::system_clock::time_point().time_since_epoch().count());

	m_BackroundItems.reserve(10);
	for (int i = 0; i < 10; i++)
		m_BackroundItems.emplace_back("cloud1.tga", Random(40, 70));

	for (BackgroundItem& sprite : m_BackroundItems)
	{
		sprite.SetPosition(Random(0, m_Boundary.x), Random(0, m_Boundary.y - sprite.GetSize().y));
	}
}

Background::~Background()
{

}

void Background::Update(float deltaTime)
{
	for (BackgroundItem& sprite : m_BackroundItems)
	{
		sprite.SetPosition(sprite.GetPosition().x - sprite.GetSpeed() * deltaTime, sprite.GetPosition().y);

		// Sprite has escaped the left side of the screen
		if (sprite.GetPosition().x + sprite.GetSize().x < 0)
		{
			sprite.SetPosition(m_Boundary.x, Random(0, m_Boundary.y - sprite.GetSize().y));
		}
	}
}

void Background::Render(ASCIIRenderer* pRenderer)
{
	for (BackgroundItem& sprite : m_BackroundItems)
		sprite.Render(pRenderer);
}