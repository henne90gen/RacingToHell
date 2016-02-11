#pragma once
#include "stdafx.h"
#include "GameObject.h"

#include <string>

class Car : public GameObject
{
public:
	Car(int health, float speed, std::string Filename);
	~Car();

	int  getHealth() { return _Health; }
	void setHealth(int Health) { _Health = Health; }

	void handleEvent(sf::Event& Event);
	void update(float FrameTime);

protected:

	int _Health;
	float _Speed;

	sf::Vector2f _Movement;
};

