#include "stdafx.h"
#include "GameObject/Boss/Mech.h"

// IDEA: Mech comes in from below and "chase" the player

Mech::Mech(sf::Texture& TextureTop, sf::Texture& TextureLegs, sf::Texture* BulletTexture) : BossCar(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), 10000, 75, TextureTop, BulletTexture),
	_TopAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), TextureTop), _LegsAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), TextureLegs), _MovementSwitch(false), _GunOrientation(-90)
{
	setSprite(_TopAnim.getSprite());
	//_LegsAnim.getSprite().setRotation(180);

	//HP-Balken
	_HealthBar.setSize(sf::Vector2f(getWidth() + 5, 5));
	_HealthBarFrame.setSize(_HealthBar.getSize());

	_DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT - 120);
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