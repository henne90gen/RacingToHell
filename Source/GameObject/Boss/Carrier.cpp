#include "stdafx.h"
#include "GameObject/Boss/Carrier.h"

Carrier::Carrier(sf::Texture & Texture, sf::Texture * BulletTexture) : BossCar(sf::Vector2f(SCREENWIDTH / 2, -1 * (float)Texture.getSize().y / 2.0f), 2000, 400, Texture, BulletTexture),
	_MovementSwitchLeftRight(false), _MovementSwitchUpDown(false), _GunPosition(sf::Vector2f(0, 0)), _Radius(), _SwitchSideTime(5.0f)
{
	_GunTexture.loadFromFile("Resources/Texture/BossCar/CannonCarrier.png");
	_GunSprite.setTexture(_GunTexture);
	_GunSprite.setOrigin(_GunTexture.getSize().x / 2, 50);

	_Speed = 200;

	_DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, 150);
	_NextPosition = _DefaultPosition;
	_Movement = Movement::DRIVETODEFAULT;
}

Carrier::~Carrier()
{
}

void Carrier::update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects)
{
	bool _Arrived = DriveToNextPosition(FrameTime);

	if (_Arrived)
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
			break;
		default:
			break;
		}
	}

	if (_Movement == Movement::SWITCHSIDES)
	{
		getSprite().setRotation((getPos().y - _DefaultPosition.y) * 180 / (SCREENHEIGHT - 2 * _DefaultPosition.y));
	}

	if (_Attack)
	{
		switch (_Pattern[_CurrentPhase].first)
		{

		default:
			break;
		}
	}

	_GunSprite.setPosition(getPos() + _GunPosition);
	_GunSprite.setRotation(_GunOrientation - 90);

	updateHealthBar();
	checkPhase();
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
	return sf::Vector2f();
}
