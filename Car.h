#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "GameObjects.h"

#include <string>

class Car : public GameObject
{
public:
	Car(int health, float speed, GameObjects Type, std::string Filename);
	~Car();

	int  getHealth() { return _Health; }
	int getSpeed() { return _Speed; }

	void setHealth(int Health) { _Health = Health; }
	void setSpeed(int Speed) { _Speed = Speed; }

	void handleEvent(sf::Event& Event);
	void update(float FrameTime);
protected:

	int _Health;
	int _Speed;

	sf::Vector2f _Movement;
};

