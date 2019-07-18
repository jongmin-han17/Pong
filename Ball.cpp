#include "Ball.h"

Ball::Ball()
	: mBallRadius(10.f)
{
	// Create the ball
	setRadius(mBallRadius - 3);
	setOutlineThickness(3);
	setOutlineColor(sf::Color::Black);
	setFillColor(sf::Color::White);
	setOrigin(mBallRadius / 2, mBallRadius / 2);
}

Ball::~Ball()
{
}

float Ball::GetBallRadius()
{
	return mBallRadius;
}
