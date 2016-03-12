#include "stdafx.h"
#include "GameObject/Boss/Mech.h"

// IDEA: Mech comes in from below and "chase" the player

Mech::Mech(sf::Texture& TextureTop, sf::Texture& TextureLegs, sf::Texture* BulletTexture) : BossCar(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), 10000, 100, TextureTop, BulletTexture),
	_TopAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), TextureTop), _LegsAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), TextureLegs), _MovementSwitch(false), _GunOrientation(-90),
	_GunRadius(81.5659f)
{
	setSprite(_TopAnim.getSprite());
	//_LegsAnim.getSprite().setRotation(180);

	//HP-Balken
	_HealthBar.setSize(sf::Vector2f(getWidth() + 5, 5));
	_HealthBarFrame.setSize(_HealthBar.getSize());

	_DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT - 200);
	_NextPosition = _DefaultPosition;
	_Movement = Movement::DRIVETODEFAULT;

	_Pattern = { std::make_pair(Phase::SPIN, 6.0f), std::make_pair(Phase::SHOTGUN, 7.0f) };
}

Mech::~Mech()
{
}

void Mech::render(sf::RenderWindow & Window)
{
	_LegsAnim.render(Window);
	_TopAnim.render(Window);

	Window.draw(_HealthBar);
	Window.draw(_HealthBarFrame);
}

void Mech::update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects)
{
	bool _Arrived = DriveToNextPosition(FrameTime);

	if (_Arrived)
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

			_GunOrientation += 60 * FrameTime;

			if (getBossEvent() == 1)
			{
				std::pair<sf::Vector2f, sf::Vector2f> Positions = calcGunPositions();

				ShootBullet(GameObjects, Positions.first, _GunOrientation);
				ShootBullet(GameObjects, Positions.second, _GunOrientation);
			}
			break;
		}
		case Phase::SHOTGUN:
		{
			_Event1Frequency = 1.6f;

			_GunOrientation = PlayerAngle(GameObjects[0]);

			if (getBossEvent() == 1)
			{
				for (int i = 0; i < 12; i++)
				{
					if (_Event1Switch)
					{
						ShootBullet(GameObjects, calcGunPositions().first, (_GunOrientation - 10) + 20 * (std::rand() % 100) / 100, 0.75 * _BulletSpeed + ((std::rand() % 100) / 100.0f * 0.5 * _BulletSpeed));
					}
					else
					{
						ShootBullet(GameObjects, calcGunPositions().second, (_GunOrientation - 10) + 20 * (std::rand() % 100) / 100, 0.75 * _BulletSpeed + ((std::rand() % 100) / 100.0f * 0.5 * _BulletSpeed));
					}
				}

				_Event1Switch = !_Event1Switch;
			}
			break;
		}
		default:
			break;
		}
	}

	_TopAnim.setRotation(_GunOrientation + 90);

	updateHealthBar();
	checkPhase();

	_LegsAnim.update(FrameTime);
	_TopAnim.update(FrameTime);

	_TopAnim.getSprite().setRotation(_TopAnim.getSprite().getRotation() + 30 * FrameTime);
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