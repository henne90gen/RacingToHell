#include "stdafx.h"
#include "Car.h"


Car::Car() : _Health(100), _Speed(1000)
{
	if (_Texture.loadFromFile("Resources/car.png")) {
		_Sprite.setTexture(_Texture);
		_Sprite.setOrigin(_Sprite.getLocalBounds().width/2, _Sprite.getLocalBounds().height/2);
	}
}


Car::~Car()
{
}

void Car::handleEvent(sf::Event& Event)
{
	/*
	if (Event.type == sf::Event::KeyPressed) {
		_Movement = sf::Vector2f(0, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			_Movement += sf::Vector2f(-1, 0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			_Movement += sf::Vector2f(1, 0);
		}
	}
	else if (Event.type == sf::Event::KeyReleased) {
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			_Movement = sf::Vector2f(0, 0);
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			_Movement = sf::Vector2f(0, 0);
		}
	} */
}

void Car::update(float FrameTime)
{
	//Vorschlag
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_Movement = sf::Vector2f(-_Speed * FrameTime, 0);

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_Movement = sf::Vector2f(_Speed * FrameTime, 0);
	}
	else
	{
		_Movement = sf::Vector2f(0, 0);
	}

	if (((getPos() + _Movement).x >= getWidth() / 2) && ((getPos() + _Movement).x <= 800 - getWidth() /2))
	{
		setPos(getPos() + _Movement);
	}

	//setPos(getPos() + (_Movement * FrameTime * _Speed));
}
