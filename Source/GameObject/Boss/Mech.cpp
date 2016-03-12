#include "stdafx.h"
#include "GameObject/Boss/Mech.h"

// IDEA: Mech comes in from below and "chase" the player

Mech::Mech(sf::Texture& TextureTop, sf::Texture& TextureLegs, sf::Texture* BulletTexture) : BossCar(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), 10000, 100, TextureTop, BulletTexture),
	_TopAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), TextureTop), _LegsAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), TextureLegs), _MovementSwitch(false), _GunOrientation(-90),
	_GunPositions(std::make_pair(sf::Vector2f(-59, 53), sf::Vector2f(59, 53))), _GunRadius(79.3)
{
	setSprite(_TopAnim.getSprite());
	//_LegsAnim.getSprite().setRotation(180);

	_DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT - 120);
	_NextPosition = _DefaultPosition;
	_Movement = Movement::DRIVETODEFAULT;

	_Pattern = { std::make_pair(Phase::SPIN, 1.0f) };
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
			_Speed = 300;
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

			_GunOrientation += 30 * FrameTime;

			if (getBossEvent() == 1)
			{
				std::pair<sf::Vector2f, sf::Vector2f> Positions = calcGunPositions();

				ShootBullet(GameObjects, Positions.first, _GunOrientation);
				ShootBullet(GameObjects, Positions.second, _GunOrientation);
			}
			break;
		}
		default:
			break;
		}
	}

	_TopAnim.getSprite().setRotation(_GunOrientation + 90);

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
	sf::Vector2f Position1 = getPos() + _GunPositions.first + sf::Vector2f(_GunRadius * std::cos(_GunOrientation / 180 * PI), _GunRadius * std::sin(_GunOrientation / 180 * PI));
	sf::Vector2f Position2 = getPos() + _GunPositions.second + sf::Vector2f(_GunRadius * std::cos((_GunOrientation + 180) / 180 * PI), _GunRadius * std::sin((_GunOrientation + 180) / 180 * PI));

	return std::make_pair(Position1, Position2);
}
