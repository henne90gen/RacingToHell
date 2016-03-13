#include "stdafx.h"
#include "GameObject/Boss/Carrier.h"

Carrier::Carrier(sf::Texture & Texture, sf::Texture & BulletTexture) : BossCar(sf::Vector2f(SCREENWIDTH / 2, -1 * (float)Texture.getSize().y / 2.0f), 2000, 200, Texture, BulletTexture),
	_MovementSwitchLeftRight(false), _MovementSwitchUpDown(false), _GunPosition(sf::Vector2f(0, 0)), _Radius(50), _SwitchSideTime(8.0f)
{
	_GunTexture.loadFromFile("Resources/Texture/BossCar/CannonCarrier.png");
	_GunSprite.setTexture(_GunTexture);
	_GunSprite.setOrigin(_GunTexture.getSize().x / 2, 50);

	_DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, 150);
	_NextPosition = _DefaultPosition;
	_Movement = Movement::DRIVETODEFAULT;

	_Pattern = { std::make_pair(Phase::BLASTSALVE, 5.0f), std::make_pair(Phase::RANDOMSPRAY, 6.0f), std::make_pair(Phase::SPIRAL, 10.0f), std::make_pair(Phase::HARDCORESPAM, 7.0f) };
}

Carrier::~Carrier()
{
}

void Carrier::update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects)
{
	if (!_IsExploding) {
		if (DriveToNextPosition(FrameTime))
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

				if (_SwitchSidesClock.getElapsedTime().asSeconds() >= _SwitchSideTime)
				{
					_Movement = Movement::SWITCHSIDES;
					_NextPosition = sf::Vector2f(getWidth() / 2 + (SCREENWIDTH - getWidth()) * (std::rand() % 100) / 100, ((int)(!_MovementSwitchUpDown) * (SCREENHEIGHT - 2 * _DefaultPosition.y)) + _DefaultPosition.y);
					_Speed = 450;
					_Attack = false;
					_MovementSwitchUpDown = !_MovementSwitchUpDown;
				}
				break;
			case Movement::SWITCHSIDES:
				_Movement = Movement::LEFTRIGHT;
				_Attack = true;
				_Speed = 200;
				_SwitchSidesClock.restart();
				_BossEventTimer1.restart();
				_BossEventTimer2.restart();
				_Event1Switch = false;
				_Event2Switch = false;
				_Event2Counter = 0;
				_Event1Counter = 0;
				break;
			default:
				break;
			}
		}

		if (_Movement == Movement::SWITCHSIDES)
		{
			getSprite().setRotation((getPos().y - _DefaultPosition.y) * 180 / (SCREENHEIGHT - 2 * _DefaultPosition.y));
		}

		_GunOrientation = PlayerAngle(GameObjects[0]);

		if (_Attack)
		{
			switch (_Pattern[_CurrentPhase].first)
			{
			case Phase::BLASTSALVE:
			{
				_Event1Frequency = 0.4f;
				_Event2Frequency = 7.0f;

				if (_Event1Switch)
				{
					if (getBossEvent() == 2)
					{
						for (int i = 2 * _Event1Counter; i <= 360; i += 20)
						{
							ShootBullet(GameObjects, getPos(), (float)i);
						}

						if (_Event1Counter + 1 < 5)
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
			}
			case Phase::SPIRAL:
			{
				_Event1Frequency = 3.0f;
				_Event2Frequency = 10.0f;

				_GunOrientation = 90;

				if (_Event1Switch)
				{
					if (getBossEvent() == 2)
					{
						for (float i = 0.0f; i < 360.0f; i += 36.0f)
						{
							sf::Vector2f Position = getPos() + sf::Vector2f(50.0f * std::cosf(i / 180 * PI), 50.0f * std::sinf(i / 180 * PI));
							float Orientation = 90 + i;

							ShootBullet(GameObjects, Position, Orientation);
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
			}
			case Phase::RANDOMSPRAY:
			{
				_Event1Frequency = 0.8f;

				if (getBossEvent() == 1)
				{
					for (float i = -30.0f; i < 240.0f; i += 10.0f)
					{
						sf::Vector2f Position = getPos() + sf::Vector2f(50.0f * std::cosf(i / 180 * PI), 50.0f * std::sinf(i / 180 * PI));
						float Orientation = (std::rand() % 270) - 30.0f;

						ShootBullet(GameObjects, Position, Orientation);
					}
				}
				break;
			}
			case Phase::HARDCORESPAM:
			{
				_Event1Frequency = 60.0f;

				if (getBossEvent() == 1)
				{
					float Orientation = (std::rand() % 360);
					_GunOrientation = Orientation;

					ShootBullet(GameObjects, calcBulletPosition(), Orientation);
				}
				break;
			}
			default:
				break;
			}
		}

		_GunSprite.setPosition(getPos() + _GunPosition);
		_GunSprite.setRotation(_GunOrientation - 90);

		updateHealthBar();
		checkPhase();
	}
	else {
		updateExplosions(FrameTime);
	}
}

void Carrier::render(sf::RenderWindow & RenderWindow)
{
	RenderWindow.draw(getSprite());
	RenderWindow.draw(_GunSprite);

	RenderWindow.draw(_HealthBar);
	RenderWindow.draw(_HealthBarFrame);
}

sf::Vector2f & Carrier::calcBulletPosition()
{
	return getPos() + _GunPosition + sf::Vector2f(_Radius * std::cos(_GunOrientation / 180 * PI), _Radius * std::sin(_GunOrientation / 180 * PI));
}
