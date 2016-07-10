#include "stdafx.h"
#include "GameObject/Car.h"

Car::Car(unsigned int id, sf::Vector2f pos, int maxhealth, float speed, GameObjectType Type, sf::Texture& texture) : 
	GameObject(id, pos, Type, texture)
{
	_Speed = speed;
	_MaxHealth = maxhealth;
	_Health = maxhealth;
	_resetColorTime = -1.0f;

	_ImpactSoundBuffer.loadFromFile("Resources/Sound/impact.wav");
}

int Car::getHealth()
{
	if (_Health < 0) {
		_Health = 0;
	}
	return _Health;
}

void Car::operator>>(std::ostream& stream) 
{
	GameObject::operator>>(stream);
	write(stream, _Speed);
	write(stream, _Health);
	write(stream, _MaxHealth);
}

void Car::operator<<(std::istream& stream) 
{
	GameObject::operator<<(stream);
	read(stream, _Speed);
	read(stream, _Health);
	read(stream, _MaxHealth);
}

void Car::operator>>(sf::Packet& packet)
{
	GameObject::operator>>(packet);
	write(packet, _Speed);
	write(packet, _Health);
	write(packet, _MaxHealth);
}

void Car::operator<<(sf::Packet& packet)
{
	GameObject::operator<<(packet);
	read(packet, _Speed);
	read(packet, _Health);
	read(packet, _MaxHealth);
}

void Car::update(float frametime, int roadspeed)
{
	if (_resetColorTime >= 0)
	{
		_resetColorTime += frametime;

		if (_resetColorTime > 0.055f)
		{
			getSprite().setColor(sf::Color::White);
			_resetColorTime = -1.0f;
		}
	}
}
