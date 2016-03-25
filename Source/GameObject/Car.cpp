#include "stdafx.h"
#include "GameObject\Car.h"


Car::Car(sf::Vector2f pos, int maxhealth, float speed, GameObjectType Type, sf::Texture& texture) : GameObject(pos, Type, texture)
{
	_Speed = speed;
	_MaxHealth = maxhealth;
	_Health = maxhealth;
}

int Car::getHealth() 
{
	if (_Health < 0) {
		_Health = 0;
	}
	return _Health;
}