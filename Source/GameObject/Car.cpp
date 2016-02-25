#include "stdafx.h"
#include "GameObject\Car.h"


Car::Car(sf::Vector2f pos, int maxhealth, float speed, GameObjects Type, sf::Texture& texture) : GameObject(pos, Type, texture)
{
	_Speed = speed;
	_MaxHealth = maxhealth;
	_Health = maxhealth;
	
}



Car::~Car()
{
}

void Car::handleEvent(sf::Event& Event)
{
}

void Car::update(float FrameTime, int RoadSpped)
{
	//getSprite().setRotation(90);
}