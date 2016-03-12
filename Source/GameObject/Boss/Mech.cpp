#include "stdafx.h"
#include "GameObject/Boss/Mech.h"


Mech::Mech(sf::Texture& TextureTop, sf::Texture& TextureLegs, sf::Texture* BulletTexture) : BossCar(sf::Vector2f(SCREENWIDTH / 2, -1 * (float)TextureTop.getSize().y / 2.0f), 2000, 400, TextureTop, BulletTexture),
	_TopAnim(sf::Vector2f(200, 200), TextureTop), _LegsAnim(sf::Vector2f(200, 200), TextureLegs), 
	_MovementSwitch(false), _GunOrientation(90)
{
	setSprite(_TopAnim.getSprite());
	_LegsAnim.getSprite().setRotation(180);

	_Speed = 300;

	_DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, 150);
	_NextPosition = _DefaultPosition;
	_Movement = Movement::DRIVETODEFAULT;
}

Mech::~Mech()
{
}

void Mech::render(sf::RenderWindow & Window)
{
	_LegsAnim.render(Window);
	_TopAnim.render(Window);
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
		default:
			break;
		}
	}
	
	_TopAnim.getSprite().setPosition(getPos());
	_LegsAnim.getSprite().setPosition(getPos());

	_TopAnim.getSprite().setRotation(_GunOrientation + 90);

	updateHealthBar();
	checkPhase();

	_LegsAnim.update(FrameTime);
	_TopAnim.update(FrameTime);

	_TopAnim.getSprite().setRotation(_TopAnim.getSprite().getRotation() + 30 * FrameTime);
}
