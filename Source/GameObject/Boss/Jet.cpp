#include "stdafx.h"
#include "GameObject\Boss\Jet.h"

Jet::Jet(sf::Texture & Texture, sf::Texture & BulletTexture) : BossCar(sf::Vector2f(-1 * Texture.getSize().x, SCREENHEIGHT / 2), 2000, 500, Texture, BulletTexture)
{
	_Traffic = true;
	_Pattern = { std::make_pair(Phase::SIDE, 6.0f) };
	_CurrentPhase = 0;

	RandomPosition();

	_Movement = Movement::STRAIGHT;

	_Pattern = {std::make_pair(Phase::SIDE, 10.0f)};
}

Jet::~Jet()
{
}

void Jet::update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects)
{
	if (DriveToNextPosition(FrameTime))
	{
		_Movement = Movement::STILL;
	}

	if (_Movement == Movement::STILL)
	{
		switch (_Pattern[_CurrentPhase].first)
		{
		case Phase::SIDE:
		{
			_Event1Frequency = 0.4f;

			if (getBossEvent() == 1)
			{
				for (int i = -40; i <= SCREENHEIGHT; i += 200)
				{
					ShootBullet(GameObjects, sf::Vector2f(0, i), 0.0f);
					ShootBullet(GameObjects, sf::Vector2f(SCREENWIDTH, i + 100), 180.0f);
				}
			}
			break;
		}
		default:
			break;
		}
	}

	if (_Movement == Movement::PARABOLA)
	{
		setPos(getPos() + sf::Vector2f(0, (float)RoadSpeed * FrameTime));
	}
	
	checkPhase();
	updateHealthBar();
}

void Jet::render(sf::RenderWindow & RenderWindow)
{
	RenderWindow.draw(getSprite());

	RenderWindow.draw(_HealthBar);
	RenderWindow.draw(_HealthBarFrame);
}

void Jet::RandomPosition()
{
	bool LeftRight = (std::rand() % 100) > 50;

	setPos(sf::Vector2f(-0.5f * getWidth() + (int)(LeftRight) * (SCREENWIDTH + getWidth()), 0.5f * getHeight() + ((std::rand() % 100) / 100.0f) * (SCREENHEIGHT - getHeight())));
	getSprite().setRotation(-180.0f * (int)(LeftRight));

	_NextPosition = getPos() +  sf::Vector2f(std::pow(-1, (int)LeftRight) * (SCREENWIDTH + getWidth()), 0);
}

void Jet::checkPhase()
{
	if (_PhaseClock.getElapsedTime().asSeconds() >= _Pattern[_CurrentPhase].second)
	{
		_Movement = Movement::STRAIGHT;
		RandomPosition();
		_PhaseClock.restart();
	}
	std::cout << _PhaseClock.getElapsedTime().asSeconds() << std::endl;
}