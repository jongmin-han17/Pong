#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>

class Paddle : public sf::RectangleShape
{
public:
	Paddle(const sf::Color& color);
	virtual ~Paddle();
	sf::Vector2f GetPaddleSize();
private:
	sf::Vector2f mPaddleSize;
};