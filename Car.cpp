#include "stdafx.h"
#include "Car.h"


Car::Car(int health, float speed, GameObjects Type, std::string Filename) : GameObject(Type, Filename)
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

}