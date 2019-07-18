#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Ball : public sf::CircleShape
{
public:
	Ball();
	virtual ~Ball();
	float GetBallRadius();
private:
	float mBallRadius;
};