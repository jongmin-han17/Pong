#include "Paddle.h"

Paddle::Paddle(const sf::Color& color)
{
	mPaddleSize.x = 25;
	mPaddleSize.y = 100;

	// Create paddle
	setSize(mPaddleSize - sf::Vector2f(3, 3));
	setOutlineThickness(3);
	setFillColor(color);
	setOutlineColor(sf::Color::Black);
	setOrigin(mPaddleSize / 2.f);


}

Paddle::~Paddle()
{
}

sf::Vector2f Paddle::GetPaddleSize()
{
	return mPaddleSize;
}
