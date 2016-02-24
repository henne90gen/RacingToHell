#include "stdafx.h"
#include "GameObject\Car.h"


Car::Car(sf::Vector2f pos, int health, float speed, GameObjects Type, sf::Texture& texture) : GameObject(pos, Type, texture)
{
	_Speed = speed;
	_Health = health;
	
}



Car::~Car()
{
}

void Car::handleEvent(sf::Event& Event)
{
}

void Car::update(float FrameTime)
{
	getSprite().setRotation(90);
}