#pragma once
#include "stdafx.h"
#include "GameObject.h"

class Car : public GameObject
{
public:
	Car(int health, float speed);
	~Car();

	sf::Vector2f getPos() { return  _Sprite.getPosition(); }
	void setPos(sf::Vector2f Pos) { _Sprite.setPosition(Pos); }
	float getWidth() { return _Sprite.getLocalBounds().width; }
	float getHeight() { return _Sprite.getLocalBounds().height; }
	int  getHealth() { return _Health; }
	void setHealth(int Health) { _Health = Health; }

protected:

	int _Health;
	float _Speed;

	sf::Vector2f _Movement;
};

