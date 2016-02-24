#include "stdafx.h"
#include "GameObject\PlayerCar.h"


PlayerCar::PlayerCar(int SelectedCar, sf::Texture& texture) : Car(sf::Vector2f(0, 0), 100, 450, GameObjects::Player, texture)
{
	setStats(SelectedCar);
	resetShotBullet();
	setPos(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT - 300));
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
		_Movement += sf::Vector2f(0, -0.45);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_Movement += sf::Vector2f(0, 1.2);
	}
	else
	{
		_Movement.y = 0;
	}

	if (Event.type == sf::Event::MouseButtonPressed)
	{
		if (_Energy - 5 >= 10)
		{
			_Energy -= 5;

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

	//Energieverbrauch
	_Energy -= 2 * FrameTime;
}

bool PlayerCar::checkForCollision(GameObject * go)
{
	if (getSprite().getGlobalBounds().intersects(go->getSprite().getGlobalBounds())) {
		return true;
	}
	return false;
}

void PlayerCar::addHealth()
{
	if (_Health + 20 > _MaxHealth)
	{
		_Health = _MaxHealth;
	}
	else
	{
		_Health += 20;
	}
}

void PlayerCar::addEnergy()
{
	if (_Energy + 50 > _MaxEnergy)
	{
		_Energy = _MaxEnergy;
	}
	else
	{
		_Energy += 50;
	}
}

void PlayerCar::setStats(int id)
{
	std::vector<int> Stats = PlayerStats::getPlayerStats(id);
	_MaxHealth = Stats[0];
	_Health = _MaxHealth;
	_MaxEnergy = Stats[1];
	_Energy = _MaxEnergy;
	_Speed = Stats[2];
	_Bulletdamage = Stats[3];
}