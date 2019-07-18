#pragma once

#include "Paddle.h"
#include "Ball.h"

class Game
{
public:
	Game();
	virtual ~Game();
	bool Init();
	void Run();
	void MovePlayerPaddle();
	void MoveAIPaddle();
	void MoveAIPaddleDirection();
	void MoveBall();
	void CheckCollisionBallScreen();
	void CheckCollisionBallPaddles();

private:
	const float PI = 3.14159f;
	const int GAME_WIDTH = 800;
	const int GAME_HEIGHT = 600;
	const sf::Time AI_TIME = sf::seconds(0.1f);
	const float PADDLE_SPEED = 400.f;
	const float BALL_SPEED = 400.f;
	sf::RenderWindow mWindow;
	Paddle* mLeftPaddle;
	Paddle* mRightPaddle;
	Ball mBall;
	sf::SoundBuffer mBallSoundBuffer;
	sf::Sound mBallSound;
	sf::Font mFont;
	sf::Text mPauseMessage;
	float mRightPaddleSpeed;
	float mBallAngle;
	float mDeltaTime;
	sf::Clock mAITimer;
	bool mbIsPlaying;
};