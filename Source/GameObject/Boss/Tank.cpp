#include "stdafx.h"
#include "GameObject/Boss/Tank.h"


Tank::Tank(sf::Texture& texture, sf::Texture& bulletTexture) : BossCar(sf::Vector2f(SCREENWIDTH / 2, -1 * (float)texture.getSize().y / 2.0f), 2000, 200, texture, bulletTexture),
	_Radius(130), _MovementSwitch(false)
{
	_GunTexture.loadFromFile("Resources/Texture/BossCar/CannonTank.png");
	_GunSprite.setTexture(_GunTexture);
	_GunSprite.setOrigin(_GunTexture.getSize().x / 2, 50);

	_GunPosition = sf::Vector2f(0, -15);
	_GunOrientation = sf::Vector2f(0, 1);
	_GunLength = _GunSprite.getLocalBounds().height - 50;

	_DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, 150);
	_NextPosition = _DefaultPosition;
	_Movement = Movement::DRIVETODEFAULT;

	_Pattern = { std::make_pair(Phase::SIMPLESHOOT, 4.0f), std::make_pair(Phase::SALVE, 10.0f), std::make_pair(Phase::SPIN, 10.0f), std::make_pair(Phase::HARDCORESPAM, 6.0f) };
	_Pattern = { std::make_pair(Phase::SALVE, 10.0f) };
}

void Tank::render(sf::RenderWindow& window)
{
	window.draw(getSprite());
	window.draw(_GunSprite);

	window.draw(_HealthBar);
	window.draw(_HealthBarFrame);

	renderExplosions(window);
}

void Tank::update(float frameTime, int roadSpeed, std::vector<GameObject*>& gameObjects)
{
	if (!_IsExploding) {
		if (driveToNextPosition(frameTime))
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
			float angle = getAngleFromVector(_GunOrientation);

			switch (_Pattern[_CurrentPhase].first) {
			/*case Phase::SIMPLESHOOT:
				_Event1Frequency = 4.0f;

				_GunOrientation = getPlayerAngle(gameObjects[0]);

				if (getBossEvent() == 1) {
					shootBullet(gameObjects, calcBulletPosition(), _GunOrientation);
				}
				break;
			*/
			case Phase::SALVE:
				/*
				_Event1Frequency = 1.0f;
				_Event2Frequency = 10.0f;

				_GunOrientation = getPlayerAngle(gameObjects[0]);

				if (_Event1Switch) {
					if (getBossEvent() == 2) {
						shootBullet(gameObjects, calcBulletPosition(), _GunOrientation);
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
				break;*/
			case Phase::SPIN:
				_Event1Frequency = 11.0f;
				if (_Event1Switch) {
					if (angle + frameTime * 100 > 180.0f) {
						_Event1Switch = false;
					}
					else {
						angle += frameTime * 100;
					}
				}
				else {
					if (angle - frameTime * 100 < 0.0f) {
						_Event1Switch = true;
					}
					else {
						angle -= frameTime * 100;
					}
				}
				_GunOrientation = sf::Vector2f(std::cosf(angle * PI / 180), std::sinf(angle * PI / 180));
				_GunOrientation = divideByLength(_GunOrientation);

				if (getBossEvent() == 1) {
					shootBullet(gameObjects, calcBulletPosition(), _GunOrientation);
				}
				break;
			case Phase::HARDCORESPAM:
				_Event1Frequency = 40.0f;
				if (getBossEvent() == 1) {
					_GunOrientation = divideByLength(sf::Vector2f(((double)(std::rand() - RAND_MAX / 2) / RAND_MAX), ((double)(std::rand() - RAND_MAX / 2) / RAND_MAX)));
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
