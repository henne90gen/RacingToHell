#include "stdafx.h"
#include "GameObject/Boss/Mech.h"

// IDEA: Mech comes in from below and "chase" the player

Mech::Mech(sf::Texture& textureTop, sf::Texture& textureLegs, sf::Texture& bulletTexture) : BossCar(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), 5000, 100, textureTop, bulletTexture),
	_TopAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), textureTop), _LegsAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), textureLegs), _MovementSwitch(false), _GunOrientation(-90),
	_GunRadius(81.5659f)
{
	setSprite(_TopAnim.getSprite());

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

void Mech::update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
	if (!_IsExploding) {
		if (DriveToNextPosition(frameTime))
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
			switch (_Pattern[_CurrentPhase].first)
			{
			case Phase::SPIN:
			{
				_Event1Frequency = 11.0f;

				_GunOrientation += 180 * frameTime;

				if (getBossEvent() == 1)
				{
					std::pair<sf::Vector2f, sf::Vector2f> Positions = calcGunPositions();

					ShootBullet(gameObjects, Positions.first, _GunOrientation);
					ShootBullet(gameObjects, Positions.second, _GunOrientation);
				}
				break;
			}
			case Phase::SHOTGUN:
			{
				_Event1Frequency = 1.5f;

				_GunOrientation = PlayerAngle(*gameObjects[0]);

				if (getBossEvent() == 1)
				{
					bool Hand = (std::rand() % 100) > 50;
					for (int i = 0; i < 10; i++)
					{

						if (Hand)
						{
							ShootBullet(gameObjects, calcGunPositions().first, (_GunOrientation - 12.5) + 25 * (std::rand() % 100) / 100, 0.75 * _BulletSpeed + ((std::rand() % 100) / 100.0f * 0.5 * _BulletSpeed));
						}
						else
						{
							ShootBullet(gameObjects, calcGunPositions().second, (_GunOrientation - 12.5) + 25 * (std::rand() % 100) / 100, 0.75 * _BulletSpeed + ((std::rand() % 100) / 100.0f * 0.5 * _BulletSpeed));
						}
					}

					_Event1Switch = !_Event1Switch;
				}
				break;
			}
			case Phase::SALVE:
			{
				_Event1Frequency = 1.5f;

				_GunOrientation = PlayerAngle(*gameObjects[0]);

				if (getBossEvent() == 1)
				{
					bool Hand = (std::rand() % 100) > 50;

					for (int i = 0; i < 5; i++)
					{
						if (Hand)
						{
							ShootBullet(gameObjects, calcGunPositions().first, _GunOrientation - 7 * (i - 2));
						}
						else
						{
							ShootBullet(gameObjects, calcGunPositions().first, _GunOrientation - 7 * (i - 2));
						}
					}
				}
				break;
			}
			default:
				break;
			}
		}

		_TopAnim.setRotation(_GunOrientation + 90);
		_LegsAnim.setRotation(_GunOrientation + 90);

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
	sf::Vector2f Position1 = getPos() + sf::Vector2f(_GunRadius * std::cos((_GunOrientation - 40.5) / 180 * PI), _GunRadius * std::sin((_GunOrientation - 40.5) / 180 * PI));
	sf::Vector2f Position2 = getPos() + sf::Vector2f(_GunRadius * std::cos((_GunOrientation + 40.5) / 180 * PI), _GunRadius * std::sin((_GunOrientation + 40.5) / 180 * PI));

	return std::make_pair(Position1, Position2);
}