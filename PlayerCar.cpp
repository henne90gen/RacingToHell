#include "stdafx.h"
#include "PlayerCar.h"


PlayerCar::PlayerCar() : Car(100, 800)
{
	//Texture laden
	//Mittelpunkt des Sprite als Ursprung festlegen
	if (_Texture.loadFromFile("Resources/car.png")) {
		_Sprite.setTexture(_Texture);
		_Sprite.setOrigin(_Sprite.getLocalBounds().width/2, _Sprite.getLocalBounds().height/2);
	}
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
}

void PlayerCar::update(float FrameTime)
{
	//_Movement anwenden - Car bewegen
	if (((getPos() + _Movement * FrameTime * _Speed).x >= getWidth() / 2) && ((getPos() + _Movement * FrameTime * _Speed).x <= 800 - getWidth() / 2))
	{
		setPos(sf::Vector2f(getPos().x + (_Movement.x * _Speed * FrameTime), getPos().y));
	}

	if (getPos().y + getHeight() / 2 + _Movement.y * FrameTime * _Speed <= 600 && getPos().y - getHeight() / 2 + _Movement.y * FrameTime * _Speed >= 0)
	{
		setPos(sf::Vector2f(getPos().x, getPos().y + _Movement.y * FrameTime * _Speed));
	}
}
