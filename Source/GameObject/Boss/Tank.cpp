#include "stdafx.h"
#include "GameObject/Boss/Tank.h"


Tank::Tank(sf::Texture& Texture, sf::Texture& BulletTexture) : BossCar(sf::Vector2f(SCREENWIDTH / 2, -1 * (float)Texture.getSize().y / 2.0f), 2000, 200, Texture, BulletTexture),
	_GunOrientation(90), _GunPosition(sf::Vector2f(0.0f, -15.0f)), _Radius(130), _MovementSwitch(false)
{
	_GunTexture.loadFromFile("Resources/Texture/BossCar/CannonTank.png");
	_GunSprite.setTexture(_GunTexture);
	_GunSprite.setOrigin(_GunTexture.getSize().x / 2, 50);

	_DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, 150);
	_NextPosition = _DefaultPosition;
	_Movement = Movement::DRIVETODEFAULT;

	_Pattern = {std::make_pair(Phase::SIMPLESHOOT, 4.0f), std::make_pair(Phase::SALVE, 10.0f), std::make_pair(Phase::SPIN, 10.0f), std::make_pair(Phase::HARDCORESPAM, 6.0f)};
}

Tank::~Tank()
{
	
}

void Tank::update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects)
{
	if (!_IsExploding) {
		if (DriveToNextPosition(FrameTime))
		{
			switch (_Movement) {
			case Movement::DRIVETODEFAULT:
				_Movement = Movement::LEFTRIGHT;
				_Speed = 200;
				_Attack = true;
				_PhaseClock.restart();
				break;
			case Movement::LEFTRIGHT:
				_MovementSwitch = !_MovementSwitch;
				if (_MovementSwitch) {
					_NextPosition = getPos() + sf::Vector2f((SCREENWIDTH - getPos().x - getWidth() / 2) * (std::rand() % 100) / 100.0f, 0.0f);
				}
				else {
					_NextPosition = getPos() - sf::Vector2f((getPos().x - getWidth() / 2) * (std::rand() % 100) / 100.0f, 0.0f);
				}
				break;
			default:
				break;
			}
		}

		if (_Attack)
		{
			switch (_Pattern[_CurrentPhase].first) {
			case Phase::SIMPLESHOOT:{
				_Event1Frequency = 4.0f;

				_GunOrientation = PlayerAngle(GameObjects[0]);

				if (getBossEvent() == 1) {
					ShootBullet(GameObjects, calcBulletPosition(), _GunOrientation);
				}
				break;
			}
			case Phase::SALVE:
				_Event1Frequency = 1.0f;
				_Event2Frequency = 10.0f;

				_GunOrientation = PlayerAngle(GameObjects[0]);

				if (_Event1Switch) {
					if (getBossEvent() == 2) {
						ShootBullet(GameObjects, calcBulletPosition(), _GunOrientation);
						if (_Event1Counter + 1 < 3) {
							_Event1Counter += 1;
						}
						else {
							_Event1Switch = false;
							_Event1Counter = 0;
						}
					}
				}
				else {
					if (getBossEvent() == 1) {
						_Event1Switch = true;
					}
				}
				break;
			case Phase::SPIN:
				_Event1Frequency = 11.0f;
				if (_GunOrientation > 180.0f || _GunOrientation < 0.0f) {
					_GunOrientation = (int)(!_Event1Switch) * 180.0f;
					_Event1Switch = !_Event1Switch;
				}
				else {
					_GunOrientation += 100.0f * FrameTime * std::pow(-1, (int)_Event1Switch);
				}

				if (getBossEvent() == 1) {
					ShootBullet(GameObjects, calcBulletPosition(), _GunOrientation);
				}
				break;
			case Phase::HARDCORESPAM:
				_Event1Frequency = 40.0f;
				if (getBossEvent() == 1) {
					float Orientation = (std::rand() % 270) - 30.0f;
					_GunOrientation = Orientation;

					ShootBullet(GameObjects, calcBulletPosition(), Orientation);
				}
				break;
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

void Tank::render(sf::RenderWindow& Window)
{
	Window.draw(getSprite());
	Window.draw(_GunSprite);

	Window.draw(_HealthBar);
	Window.draw(_HealthBarFrame);

	renderExplosions(Window);
}

sf::Vector2f& Tank::calcBulletPosition()
{
	return getPos() + _GunPosition + sf::Vector2f(_Radius * std::cos(_GunOrientation / 180 * PI), _Radius * std::sin(_GunOrientation / 180 * PI));
}
