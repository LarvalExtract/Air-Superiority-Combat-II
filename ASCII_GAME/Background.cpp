#include "Background.h"
#include "Core/Utils.h"

#include <chrono>

const int MAX_CLOUDS = 1;
Texture clouds[MAX_CLOUDS] = { TGAFile("cloud1.tga") };


Background::Background(int width, int height) :
	m_Boundary(width, height),
	m_BackgroundItems(10)
{
	srand(std::chrono::system_clock::time_point().time_since_epoch().count());

	for (int i = 0; i < m_BackgroundItems.size(); i++)
	{
		// Select a random cloud texture
		m_BackgroundItems[i].SetTexture(clouds[Random(0, MAX_CLOUDS)]);	

		// Set a random speed for the background item
		m_BackgroundItems[i].SetSpeed(Random(30, 70));

		// Initialise the background item to a random position on screen
		m_BackgroundItems[i].SetPosition(Random(0, m_Boundary.x), Random(0, m_Boundary.y - m_BackgroundItems[i].GetSize().y));
	}
}

Background::~Background()
{

}

void Background::Update(float deltaTime)
{
	for (BackgroundItem& sprite : m_BackgroundItems)
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
	for (BackgroundItem& sprite : m_BackgroundItems)
		sprite.Render(pRenderer);
}