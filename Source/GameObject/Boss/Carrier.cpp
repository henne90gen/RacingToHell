#include "stdafx.h"
#include "GameObject/Boss/Carrier.h"

//TODO: Change health of Carrier back to 2000
Carrier::Carrier(int difficulty, int HP, sf::Texture & texture, sf::Texture & bulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBufferShot, sf::SoundBuffer &soundBufferExplosion,float Volume) : 
	BossCar(sf::Vector2f(SCREENWIDTH / 2, -1 * (float)texture.getSize().y / 2.0f), difficulty, HP, 200, texture, bulletTexture, soundEffects, soundBufferShot, soundBufferExplosion, Volume),
	_MovementSwitchLeftRight(false), _MovementSwitchUpDown(false), _Radius(50), _SwitchSideTime(8.0f), _SwitchSides(false)
{
	_GunTexture.loadFromFile("Resources/Texture/BossCar/CannonCarrier.png");
	_GunSprite.setTexture(_GunTexture);
	_GunSprite.setOrigin(_GunTexture.getSize().x / 2, 50);

	_GunPosition = sf::Vector2f(0, 0);
	_GunLength = _GunSprite.getLocalBounds().height - 50;

	_DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, 150);
	_NextPosition = _DefaultPosition;
	_Movement = Movement::DRIVETODEFAULT;

	_Pattern = { std::make_pair(Phase::BLASTSALVE, 5.0f), std::make_pair(Phase::NOTHING, 1.5f), std::make_pair(Phase::RANDOMSPRAY, 6.0f), std::make_pair(Phase::NOTHING, 1.5f),
		std::make_pair(Phase::SPIRAL, 8.0f), std::make_pair(Phase::NOTHING, 2.0f), std::make_pair(Phase::HARDCORESPAM, 7.0f), std::make_pair(Phase::NOTHING, 1.5f) };
}

void Carrier::render(sf::RenderWindow & window)
{
	window.draw(getSprite());
	window.draw(_GunSprite);

	window.draw(_HealthBar);
	window.draw(_HealthBarFrame);

	renderExplosions(window);
}

void Carrier::update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
	if (!_IsExploding) {
		if (driveToNextPosition(frameTime))
		{
			switch (_Movement)
			{
			case Movement::DRIVETODEFAULT:
				_Movement = Movement::LEFTRIGHT;
				_Speed = 200;
				_Attack = true;
				_PhaseClock.restart();
				_SwitchSidesClock.restart();
				break;
			case Movement::LEFTRIGHT:
				_Attack = true;
				_MovementSwitchLeftRight = !_MovementSwitchLeftRight;

				if (_MovementSwitchLeftRight)
				{
					_NextPosition = getPos() + sf::Vector2f((SCREENWIDTH - getPos().x - getWidth() / 2) * (std::rand() % 100) / 100.0f, 0.0f);
				}
				else
				{
					_NextPosition = getPos() - sf::Vector2f((getPos().x - getWidth() / 2) * (std::rand() % 100) / 100.0f, 0.0f);
				}
				break;
			case Movement::SWITCHSIDES:
				_Movement = Movement::LEFTRIGHT;
				_MovementSwitchUpDown = !_MovementSwitchUpDown;
				_Attack = true;
				_Speed = 200;
				_SwitchSidesClock.restart();
				_BossEventTimer1.restart();
				_BossEventTimer2.restart();
				_Event1Switch = false;
				_Event2Switch = false;
				_Event2Counter = 0;
				_Event1Counter = 0;

				_PhaseClock.restart();

				break;
			}

			//if (_SwitchSidesClock.getElapsedTime().asSeconds() >= _SwitchSideTime)
			
		}

		if (_SwitchSides)
		{
			_Movement = Movement::SWITCHSIDES;
			_NextPosition = sf::Vector2f(getWidth() / 2 + (SCREENWIDTH - getWidth()) * (std::rand() % 100) / 100, ((int)(!_MovementSwitchUpDown) * (SCREENHEIGHT - 2 * _DefaultPosition.y)) + _DefaultPosition.y);
			_Speed = 450;
			_Attack = false;
			_SwitchSides = false;
		}

		if (_Movement == Movement::SWITCHSIDES)
		{
			getSprite().setRotation((getPos().y - _DefaultPosition.y) * 180 / (SCREENHEIGHT - 2 * _DefaultPosition.y));
		}

		_GunOrientation = divideByLength(gameObjects[0]->getPos() - getPos());

		if (_Attack)
		{
			switch (_Pattern[_CurrentPhase].first)
			{
			case Phase::BLASTSALVE:
				_Event1Frequency = 0.4f;
				_Event2Frequency = 7.0f;

				if (_Event1Switch)
				{
					if (getBossEvent() == 2)
					{
						for (float i = (2 * _Event1Counter) * PI / 180; i <= 2 * PI; i += PI / (7 + 2 *_Difficulty))
						{
							sf::Vector2f orientation = sf::Vector2f(std::cosf(i), std::sinf(i));
							shootBullet(gameObjects, getPos(), orientation);
						}

						if (_Event1Counter + 1 < 4 + _Difficulty)
						{
							_Event1Counter += 1;
						}
						else
						{
							_Event1Switch = false;
							_Event1Counter = 0;
						}
					}
				}
				else
				{
					if (getBossEvent() == 1)
					{
						_Event1Switch = true;
					}
				}
				break;
			case Phase::SPIRAL:
				_Event1Frequency = 3.0f;
				_Event2Frequency = 10.0f;

				if (_Event1Switch)
				{
					if (getBossEvent() == 2)
					{
						for (float i = 0.0f; i < 2 * PI; i += PI / (4 + _Difficulty))
						{
							_GunOrientation = sf::Vector2f(std::cosf(i), std::sinf(i));
							shootBullet(gameObjects, calcBulletPosition(), _GunOrientation);
							_GunOrientation = divideByLength(gameObjects[0]->getPos() - getPos());
						}
					}
				}
				else
				{
					if (getBossEvent() == 1)
					{
						_Event1Switch = true;
					}
				}
				break;
			case Phase::RANDOMSPRAY:
				_Event1Frequency = 0.8f + 0.10f * (float)_Difficulty;

				if (getBossEvent() == 1) {
					for (float i = PI * (float)_MovementSwitchUpDown; i < PI + PI * (float) _MovementSwitchUpDown; i += PI / (10 + 3 * _Difficulty)) {
						_GunOrientation = sf::Vector2f(std::cosf(i), std::sinf(i));
						sf::Vector2f orientation = divideByLength(sf::Vector2f(((float)(std::rand() - (float)(RAND_MAX) / 2) / (float)(RAND_MAX)), (float)(std::rand() / (float)(RAND_MAX)) * std::pow(-1, (int)(_MovementSwitchUpDown))));
						shootBullet(gameObjects, calcBulletPosition(), orientation);
					}
				}
				break;
			
			case Phase::HARDCORESPAM:
				_Event1Frequency = 40.0f + 15.0f * (float)_Difficulty;

				if (getBossEvent() == 1) {
					_GunOrientation = divideByLength(sf::Vector2f(((float)(std::rand() - (float)(RAND_MAX) / 2) / (float)(RAND_MAX)),
						((float)(std::rand() - (float)(RAND_MAX) / 2) / (float)(RAND_MAX))));
					shootBullet(gameObjects, calcBulletPosition(), _GunOrientation);
				}
				break;
			}
		}

		_GunSprite.setPosition(getPos() + _GunPosition);
		_GunSprite.setRotation(getAngleFromVector(_GunOrientation) - 90);

		updateHealthBar();
		checkPhase();
	}
	else {
		updateExplosions(frameTime);
	}
}

void Carrier::checkPhase()
{
	if (_PhaseClock.getElapsedTime().asSeconds() > _Pattern[_CurrentPhase].second)
	{
		if (_Pattern[_CurrentPhase].first != Phase::NOTHING && std::rand() % RAND_MAX > 0.5f *  RAND_MAX)
		{
			_SwitchSides = true;
		}

		if (_CurrentPhase + 1 >= _Pattern.size())
		{
			_CurrentPhase = 0;
		}
		else
		{
			_CurrentPhase++;
		}

		_BossEventTimer1.restart();
		_BossEventTimer2.restart();
		_Event1Switch = false;
		_Event2Switch = false;
		_Event2Counter = 0;
		_Event1Counter = 0;

		_PhaseClock.restart();
	}
}

/*sf::Vector2f Carrier::calcBulletPosition()
{
	//return getPos() + _GunPosition + sf::Vector2f(_Radius * std::cos(_GunOrientation / 180 * PI), _Radius * std::sin(_GunOrientation / 180 * PI));
	return sf::Vector2f();
}*/
