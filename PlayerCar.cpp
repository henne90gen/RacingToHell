#include "stdafx.h"
#include "PlayerCar.h"


PlayerCar::PlayerCar(int HP, int Speed) : Car(HP, Speed, GameObjects::Player, "car")
{
	setPos(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT - getHeight() / 2));
	_Energy = 100;
	resetShotBullet();
}


PlayerCar::~PlayerCar()
{

}

void PlayerCar::handleEvent(sf::Event& Event)
{
	//Keyinput abfragen und in _Movement speichern
	_Movement = sf::Vector2f(0, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		_Movement += sf::Vector2f(-1, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		_Movement += sf::Vector2f(1, 0);
	}
	else {
		_Movement = sf::Vector2f(0, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_Movement += sf::Vector2f(0, -0.4);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_Movement += sf::Vector2f(0, 1.3);
	}
	else
	{
		_Movement.y = 0;
	}

	if (Event.type == sf::Event::MouseButtonPressed)
	{
		if (_Energy - 10 >= 0)
		{
			_Energy -= 10;

			if (getPos().x > Event.mouseButton.x)
			{
				_ShotBullet = std::atanf((Event.mouseButton.y - getPos().y) / (Event.mouseButton.x - getPos().x)) * 180.0f / PI + 180;
			}
			else if (getPos().x < Event.mouseButton.x)
			{
				_ShotBullet = std::atanf((getPos().y - Event.mouseButton.y) / (getPos().x - Event.mouseButton.x)) * 180.0f / PI;
			}
			else
			{
				if (Event.mouseButton.y > getPos().y)
				{
					_ShotBullet = 90;
				}
				else
				{
					_ShotBullet = -90;
				}
			}
		}
	}
}

void PlayerCar::update(float FrameTime)
{
	//_Movement anwenden - Car bewegen
	if (((getPos() + _Movement * FrameTime * (float)_Speed).x >= getWidth() / 2) && ((getPos() + _Movement * FrameTime * (float)_Speed).x <= SCREENWIDTH - getWidth() / 2))
	{
		setPos(sf::Vector2f(getPos().x + (_Movement.x * _Speed * FrameTime), getPos().y));
	}

	if (getPos().y + getHeight() / 2 + _Movement.y * FrameTime * _Speed <= SCREENHEIGHT && getPos().y - getHeight() / 2 + _Movement.y * FrameTime * _Speed >= 0)
	{
		setPos(sf::Vector2f(getPos().x, getPos().y + _Movement.y * FrameTime * _Speed));
	}
}

bool PlayerCar::checkForCollision(GameObject * go)
{
	if (getSprite().getGlobalBounds().intersects(go->getSprite().getGlobalBounds())) {
		//std::cout << "We are intersecting!" << std::endl;
		return true;
	}
	return false;
}
