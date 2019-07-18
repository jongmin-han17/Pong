#include "Game.h"

Game::Game()
	: mRightPaddleSpeed(0.f)
	, mBallAngle(0.f)
	, mLeftPaddle(nullptr)
	, mRightPaddle(nullptr)
	, mbIsPlaying(false)
{

}

Game::~Game()
{
	delete mLeftPaddle;
	delete mRightPaddle;
}

bool Game::Init()
{
	mLeftPaddle = new Paddle(sf::Color(100, 100, 200));
	mRightPaddle = new Paddle(sf::Color(200, 100, 100));
	// Create window of the application
	mWindow.create(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT, 32), "SFML pong", sf::Style::Titlebar | sf::Style::Close);
	mWindow.setVerticalSyncEnabled(true);

	// Load the sounds used in the game
	if (!mBallSoundBuffer.loadFromFile("resources/ball.wav"))
	{
		return false;
	}
	mBallSound.setBuffer(mBallSoundBuffer);


	// Load the text font
	if (!mFont.loadFromFile("resources/sansation.ttf"))
	{
		return false;
	}

	// Initialize the pause message
	mPauseMessage.setFont(mFont);
	mPauseMessage.setCharacterSize(40);
	mPauseMessage.setPosition(170.f, 150.f);
	mPauseMessage.setFillColor(sf::Color::White);
	mPauseMessage.setString("Welcome to Ping Pong!\nPress space to start the game");

	return true;
}

void Game::Run()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	sf::Clock clock;

	while (mWindow.isOpen())
	{
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			// Window closed or escape key pressed: exit
			if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			{
				mWindow.close();
				break;
			}

			// Space key pressed: play
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
			{
				if (!mbIsPlaying)
				{
					// (re)start the game
					mbIsPlaying = true;
					clock.restart();

					// Reset the position of the paddles and ball
					mLeftPaddle->setPosition(static_cast<float>(10 + mLeftPaddle->GetPaddleSize().x / 2), static_cast<float>(GAME_HEIGHT / 2));
					mRightPaddle->setPosition(static_cast<float>(GAME_WIDTH - 10 - mRightPaddle->GetPaddleSize().x / 2), static_cast<float>(GAME_HEIGHT / 2));
					mBall.setPosition(static_cast<float>(GAME_WIDTH / 2), static_cast<float>(GAME_HEIGHT / 2));

					// Reset the ball angle
					do
					{
						// Make sure the ball initial angle is not too much vertical
						mBallAngle = (std::rand() % 360) * 2 * PI / 360;
					} while (std::abs(std::cos(mBallAngle)) < 0.7f);
				}
			}
		}

		if (mbIsPlaying)
		{
			mDeltaTime = clock.restart().asSeconds();
			MovePlayerPaddle();
			MoveAIPaddle();
			MoveAIPaddleDirection();
			MoveBall();
			CheckCollisionBallScreen();
			CheckCollisionBallPaddles();
		}

		// Clear the window
		mWindow.clear(sf::Color(50, 200, 50));

		if (mbIsPlaying)
		{
			// Draw the paddles and the ball
			mWindow.draw(*mLeftPaddle);
			mWindow.draw(*mRightPaddle);
			mWindow.draw(mBall);
		}
		else
		{
			// Draw the pause message
			mWindow.draw(mPauseMessage);
		}

		// Display things on screen
		mWindow.display();
	}
}

void Game::MovePlayerPaddle()
{

	// Move the player's paddle
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (mLeftPaddle->getPosition().y - mLeftPaddle->GetPaddleSize().y / 2 > 5.f))
	{
		std::cout << mLeftPaddle->getPosition().y - mLeftPaddle->GetPaddleSize().y / 2 << std::endl;
		mLeftPaddle->move(0.f, -PADDLE_SPEED * mDeltaTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (mLeftPaddle->getPosition().y + mLeftPaddle->GetPaddleSize().y / 2 < GAME_HEIGHT - 5.f))
	{
		mLeftPaddle->move(0.f, PADDLE_SPEED * mDeltaTime);
	}
}

void Game::MoveAIPaddle()
{
	// Move the computer's paddle
	if (((mRightPaddleSpeed < 0.f) && (mRightPaddle->getPosition().y - mRightPaddle->GetPaddleSize().y / 2 > 5.f)) || ((mRightPaddleSpeed > 0.f) && (mRightPaddle->getPosition().y + mRightPaddle->GetPaddleSize().y / 2 < GAME_HEIGHT - 5.f)))
	{
		mRightPaddle->move(0.f, mRightPaddleSpeed * mDeltaTime);
	}
}

void Game::MoveAIPaddleDirection() // Update the computer's paddle direction according to the ball position
{
	if (mAITimer.getElapsedTime() > AI_TIME)
	{
		mAITimer.restart();
		if (mBall.getPosition().y + mBall.GetBallRadius() > mRightPaddle->getPosition().y + mRightPaddle->GetPaddleSize().y / 2)
		{
			mRightPaddleSpeed = PADDLE_SPEED;
		}
		else if (mBall.getPosition().y - mBall.GetBallRadius() < mRightPaddle->getPosition().y - mRightPaddle->GetPaddleSize().y / 2)
		{
			mRightPaddleSpeed = -PADDLE_SPEED;
		}
		else
		{
			mRightPaddleSpeed = 0.f;
		}
	}
}

void Game::MoveBall()
{
	float factor = BALL_SPEED * mDeltaTime;
	mBall.move(std::cos(mBallAngle) * factor, std::sin(mBallAngle) * factor);
}

void Game::CheckCollisionBallScreen()
{
	if (mBall.getPosition().x - mBall.GetBallRadius() < 0.f)
	{
		mbIsPlaying = false;
		mPauseMessage.setString("You lost!\nPress space to restart or\nescape to exit");
	}
	if (mBall.getPosition().x + mBall.GetBallRadius() > GAME_WIDTH)
	{
		mbIsPlaying = false;
		mPauseMessage.setString("You won!\nPress space to restart or\nescape to exit");
	}
	if (mBall.getPosition().y - mBall.GetBallRadius() < 0.f)
	{

		mBallSound.play();
		mBallAngle = -mBallAngle;
		mBall.setPosition(mBall.getPosition().x, mBall.GetBallRadius() + 0.1f);
	}
	if (mBall.getPosition().y + mBall.GetBallRadius() > GAME_HEIGHT)
	{
		mBallSound.play();
		mBallAngle = -mBallAngle;
		mBall.setPosition(mBall.getPosition().x, GAME_HEIGHT - mBall.GetBallRadius() - 0.1f);
	}
}

void Game::CheckCollisionBallPaddles()
{
	// Left Paddle
	if (mBall.getPosition().x - mBall.GetBallRadius() < mLeftPaddle->getPosition().x + mLeftPaddle->GetPaddleSize().x / 2 &&
		mBall.getPosition().x - mBall.GetBallRadius() > mLeftPaddle->getPosition().x &&
		mBall.getPosition().y + mBall.GetBallRadius() >= mLeftPaddle->getPosition().y - mLeftPaddle->GetPaddleSize().y / 2 &&
		mBall.getPosition().y - mBall.GetBallRadius() <= mLeftPaddle->getPosition().y + mLeftPaddle->GetPaddleSize().y / 2)
	{
		if (mBall.getPosition().y > mLeftPaddle->getPosition().y)
			mBallAngle = PI - mBallAngle + (std::rand() % 20) * PI / 180;
		else
			mBallAngle = PI - mBallAngle - (std::rand() % 20) * PI / 180;

		mBallSound.play();
		mBall.setPosition(mLeftPaddle->getPosition().x + mBall.GetBallRadius() + mLeftPaddle->GetPaddleSize().x / 2 + 0.1f, mBall.getPosition().y);
	}

	// Right Paddle
	if (mBall.getPosition().x + mBall.GetBallRadius() > mRightPaddle->getPosition().x - mRightPaddle->GetPaddleSize().x / 2 &&
		mBall.getPosition().x + mBall.GetBallRadius() < mRightPaddle->getPosition().x &&
		mBall.getPosition().y + mBall.GetBallRadius() >= mRightPaddle->getPosition().y - mRightPaddle->GetPaddleSize().y / 2 &&
		mBall.getPosition().y - mBall.GetBallRadius() <= mRightPaddle->getPosition().y + mRightPaddle->GetPaddleSize().y / 2)
	{
		if (mBall.getPosition().y > mRightPaddle->getPosition().y)
			mBallAngle = PI - mBallAngle + (std::rand() % 20) * PI / 180;
		else
			mBallAngle = PI - mBallAngle - (std::rand() % 20) * PI / 180;

		mBallSound.play();
		mBall.setPosition(mRightPaddle->getPosition().x - mBall.GetBallRadius() - mRightPaddle->GetPaddleSize().x / 2 - 0.1f, mBall.getPosition().y);
	}
}

