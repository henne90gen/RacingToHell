#include "stdafx.h"
#include "GameObject/Boss/Mech.h"

// IDEA: Mech comes in from below and "chase" the player

Mech::Mech(sf::Texture& textureTop, sf::Texture& textureLegs, sf::Texture& bulletTexture) : BossCar(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), 5000, 100, textureTop, bulletTexture),
	_TopAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), textureTop), _LegsAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), textureLegs), _MovementSwitch(false), _GunRadius(81.5659f)
{
	setSprite(_TopAnim.getSprite());

	_GunOrientation = sf::Vector2f(0, -1);
	_GunPosition = sf::Vector2f(62, 0);
	_GunLength = 53.0f;

	//HP-Balken
	_HealthBar.setSize(sf::Vector2f(getWidth() + 5, 5));
	_HealthBarFrame.setSize(_HealthBar.getSize());

	_DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT - 200);
	_NextPosition = _DefaultPosition;
	_Movement = Movement::DRIVETODEFAULT;

	_Pattern = { std::make_pair(Phase::SPIN, 2.0f), std::make_pair(Phase::SHOTGUN, 7.0f), std::make_pair(Phase::SALVE, 7.0f) };
}

void Mech::render(sf::RenderWindow & window)
{
	_LegsAnim.render(window);
	_TopAnim.render(window);

	window.draw(_HealthBar);
	window.draw(_HealthBarFrame);

	renderExplosions(window);
}

void Mech::update(float frameTime, int roadSpeed, std::vector<GameObject*>& gameObjects)
{
	float gunPosLength = std::sqrtf(std::pow(_GunPosition.x, 2) + std::pow(_GunPosition.y, 2));
	_GunPosition = sf::Vector2f(cosf(std::atanf(_GunOrientation.y / _GunOrientation.x) + PI / 2), 
								sinf(std::atanf(_GunOrientation.y / _GunOrientation.x) + PI / 2)) * gunPosLength;

	if (!_IsExploding) {
		if (driveToNextPosition(frameTime))
		{
			switch (_Movement)
			{
			case BossCar::DRIVETODEFAULT:
				_Movement = Movement::LEFTRIGHT;
				_Attack = true;
				_PhaseClock.restart();
				break;
			case BossCar::LEFTRIGHT:
				_MovementSwitch = !_MovementSwitch;

				if (_MovementSwitch)
				{
					_NextPosition = getPos() + sf::Vector2f((SCREENWIDTH - getPos().x - getWidth() / 2) * (std::rand() % 100) / 100.0f, 0.0f);
				}
				else
				{
					_NextPosition = getPos() - sf::Vector2f((getPos().x - getWidth() / 2) * (std::rand() % 100) / 100.0f, 0.0f);
				}
				break;
			default:
				break;
			}
		}

		if (_Attack)
		{

			_GunOrientation = divideByLength(gameObjects[0]->getPos() - getPos());

			switch (_Pattern[_CurrentPhase].first)
			{
			case Phase::SPIN:
			{
				_Event1Frequency = 11.0f;

				float gunAngle = getAngleFromVector(_GunOrientation) + 180 * frameTime;
				_GunOrientation = sf::Vector2f(std::cosf(gunAngle * PI / 180), std::sinf(gunAngle * PI / 180));

				if (getBossEvent() == 1)
				{
					std::pair<sf::Vector2f, sf::Vector2f> Positions = calcGunPositions();

					shootBullet(gameObjects, Positions.first, _GunOrientation);
					shootBullet(gameObjects, Positions.second, _GunOrientation);
				}
				break;
			}
			case Phase::SHOTGUN:
				_Event1Frequency = 1.5f;

				if (getBossEvent() == 1)
				{
					bool Hand = (std::rand() % 100) > 50;
					for (int i = 0; i < 10; i++)
					{
						float bulAngle = (getAngleFromVector(_GunOrientation) * PI / 180 - PI / 28.8) + PI / 14.4 * ((std::rand() % 100) / 100.0f);
						sf::Vector2f bulOrientation = divideByLength(sf::Vector2f(std::cosf(bulAngle), std::sinf(bulAngle)));
						if (Hand)
						{
							shootBullet(gameObjects, calcGunPositions().first, bulOrientation, 0.75 * _BulletSpeed + ((std::rand() % 100) / 100.0f * 0.5 * _BulletSpeed));
						}
						else
						{
							shootBullet(gameObjects, calcGunPositions().second, bulOrientation, 0.75 * _BulletSpeed + ((std::rand() % 100) / 100.0f * 0.5 * _BulletSpeed));
						}
					}

					_Event1Switch = !_Event1Switch;
				}
				break;
			case Phase::SALVE:
				_Event1Frequency = 1.5f;

				if (getBossEvent() == 1)
				{
					bool Hand = (std::rand() % 100) > 50;

					for (int i = 0; i < 5; i++)
					{
						if (Hand)
						{
							shootBullet(gameObjects, calcGunPositions().first, _GunOrientation);
						}
						else
						{
							shootBullet(gameObjects, calcGunPositions().second, _GunOrientation);
						}
					}
				}
				break;
			}
			
		}

		_TopAnim.setRotation(getAngleFromVector(_GunOrientation) + 90);
		_LegsAnim.setRotation(getAngleFromVector(_GunOrientation) + 90);

		updateHealthBar();

		if (_Movement != Movement::DRIVETODEFAULT)
		{
			checkPhase();
		}

		_LegsAnim.update(frameTime);
		_TopAnim.update(frameTime);
	}
	else {
		updateExplosions(frameTime);
	}
}

void Mech::setPos(sf::Vector2f pos) {
	_TopAnim.getSprite().setPosition(pos);
	_LegsAnim.getSprite().setPosition(pos);
}

std::pair<sf::Vector2f, sf::Vector2f>& Mech::calcGunPositions()
{
	sf::Vector2f pos1 = getPos() - _GunPosition + _GunOrientation * _GunLength;
	sf::Vector2f pos2 = getPos() + _GunPosition + _GunOrientation * _GunLength;
	return std::make_pair(pos1, pos2);
}