#include "stdafx.h"
#include "GameObject\Car.h"

Car::Car(sf::Vector2f pos, int maxhealth, float speed, GameObjectType Type, sf::Texture& texture) : GameObject(pos, Type, texture)
{
	_Speed = speed;
	_MaxHealth = maxhealth;
	_Health = maxhealth;
}

Car::Car(std::istream& stream, GameObjectType type, sf::Texture& texture) : GameObject(sf::Vector2f(), type, texture) 
{
	*this << stream;
}

void Car::operator>>(std::ostream& stream) 
{
	write(stream, getType());
	write(stream, getPos().x);
	write(stream, getPos().y);
	write(stream, _Speed);
	write(stream, _Health);
	write(stream, _MaxHealth);
}

void Car::operator<<(std::istream& stream) 
{
	float x, y;
	read(stream, x);
	read(stream, y);
	setPos(sf::Vector2f(x, y));
	read(stream, _Speed);
	read(stream, _Health);
	read(stream, _MaxHealth);
}