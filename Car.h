#pragma once
#include "stdafx.h"
#include "GameObject.h"

class Car : public GameObject
{
public:
	Car(int health, int speed);
	~Car();

protected:

	int _Health;
	float _Speed;

	sf::Vector2f _Movement;
};

