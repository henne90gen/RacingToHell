#include "stdafx.h"
#include "Car.h"


Car::Car(int health, float speed, std::string Filename) : GameObject(Filename)
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