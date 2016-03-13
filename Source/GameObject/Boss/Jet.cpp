#include "stdafx.h"
#include "GameObject\Boss\Jet.h"

Jet::Jet(sf::Texture & Texture, sf::Texture & BulletTexture) : BossCar(sf::Vector2f(-1 * Texture.getSize().x, SCREENHEIGHT / 2), 2000, 500, Texture, BulletTexture)
{
	_Traffic = true;
	_Pattern = { std::make_pair(Phase::SIDE, 6.0f) };
	_CurrentPhase = 0;

	RandomPosition();

	_Movement = Movement::STRAIGHT;

	_Pattern = {std::make_pair(Phase::SIDE, 10.5f)};
}

Jet::~Jet()
{
}

void Jet::update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects)
{
	if (_Movement != Movement::STILL && DriveToNextPosition(FrameTime))
	{
		_BossEventTimer1.restart();
		_BossEventTimer2.restart();
		_Event1Switch = false;
		_Event2Switch = false;
		_Event2Counter = 0;
		_Event1Counter = 0;

		_Movement = Movement::STILL;
	}

	if (_Movement == Movement::STILL)
	{
		switch (_Pattern[_CurrentPhase].first)
		{
		case Phase::SIDE:
		{
			_Event1Frequency = 0.2f;

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
		case Phase::POWERUPLANES:
		{
			_Event1Frequency = 0.2f;
			_Event2Frequency = 1.0f;

			if (getBossEvent() == 1 || _Event2Counter > 0)
			{
				if (getBossEvent() == 2)
				{
					if (_Event2Counter + 1 <= 3)
					{

						++_Event2Counter;
					}
					else
					{
						_Event2Counter = 0;
					}
				}
			}
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
		if (_CurrentPhase + 1 >= _Pattern.size())
		{
			_CurrentPhase = 0;
		}
		else
		{
			++_CurrentPhase;
		}

		_Movement = Movement::STRAIGHT;
		RandomPosition();
		_PhaseClock.restart();
	}
}