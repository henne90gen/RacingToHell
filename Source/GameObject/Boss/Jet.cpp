#include "stdafx.h"
#include "GameObject\Boss\Jet.h"

Jet::Jet(sf::Texture & texture, sf::Texture & bulletTexture) : BossCar(sf::Vector2f(-1 * texture.getSize().x, SCREENHEIGHT / 2), 2000, 500, texture, bulletTexture)
{
	_Traffic = true;
	_Pattern = { std::make_pair(Phase::SIDE, 6.0f) };
	_CurrentPhase = 0;

	randomPosition();

	_Movement = Movement::STRAIGHT;

	_Pattern = {std::make_pair(Phase::SIDE, 10.5f), std::make_pair(Phase::SAVELANES, 10.5f)};
}

void Jet::render(sf::RenderWindow & window)
{
	window.draw(getSprite());

	window.draw(_HealthBar);
	window.draw(_HealthBarFrame);

	renderExplosions(window);
}

void Jet::update(float frameTime, int roadSpeed, std::vector<GameObject*>& gameObjects)
{
	if (!_IsExploding) {
		if (_Movement != Movement::STILL && driveToNextPosition(frameTime))
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
						shootBullet(gameObjects, sf::Vector2f(0, i), sf::Vector2f(1, 0));
						shootBullet(gameObjects, sf::Vector2f(SCREENWIDTH, i + 100), sf::Vector2f(-1, 0));
					}
				}
				break;
			}
			case Phase::SAVELANES:
			{
				_Event1Frequency = 0.2f;
				_Event2Frequency = 0.0f;

				if (getBossEvent() == 1 || _Event2Counter > 0)
				{
					_Event2Frequency = 3.0f;

					if (getBossEvent() == 2)
					{
						if (_Event2Counter + 1 <= 3)
						{
							for (int i = 0; i < 3; i++)
							{
								shootBullet(gameObjects, sf::Vector2f(i * 150 + 150, 0), sf::Vector2f(0, 1));
							}

							shootBullet(gameObjects, sf::Vector2f(20, 0), sf::Vector2f(0, 1));
							shootBullet(gameObjects, sf::Vector2f(SCREENWIDTH - 20, 0), sf::Vector2f(0, 1));

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
			setPos(getPos() + sf::Vector2f(0, (float)roadSpeed * frameTime));
		}

		checkPhase();
		updateHealthBar();
	}
	else {
		updateExplosions(frameTime);
	}
}

void Jet::randomPosition()
{
	bool LeftRight = (std::rand() % 100) > 50;

	setPos(sf::Vector2f(-0.5f * getWidth() + (int)(LeftRight) * (SCREENWIDTH + getWidth()), 0.5f * getHeight() + ((std::rand() % 100) / 100.0f) * (SCREENHEIGHT - getHeight())));
	getSprite().setRotation(-180.0f * (int)(LeftRight));

	_NextPosition = getPos() +  sf::Vector2f(std::pow(-1, (int)LeftRight) * (SCREENWIDTH + getWidth()), 0);
}

void Jet::checkPhase()
{
	if (_PhaseClock.getElapsedTime().asSeconds() >= _Pattern[_CurrentPhase].second) {
		if (_CurrentPhase + 1 >= _Pattern.size()) {
			_CurrentPhase = 0;
		}
		else {
			++_CurrentPhase;
		}

		_Movement = Movement::STRAIGHT;
		randomPosition();
		_PhaseClock.restart();
	}
}