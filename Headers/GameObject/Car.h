#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "GameObjects.h"

#include <string>

class Car : public GameObject
{
public:
	Car(sf::Vector2f pos, int maxhealth, float speed, GameObjects Type, sf::Texture& texture);
	~Car();

	int getHealth() { return _Health; }
	int getMaxHealth() { return _MaxHealth; }
	int getSpeed() { return _Speed; }

	void setHealth(int Health) { _Health = Health; }
	void setSpeed(int Speed) { _Speed = Speed; }

	void takeDamage(int damage) { _Health -= damage; }

	void handleEvent(sf::Event& Event);
	void update(float FrameTime, int RoadSpeed);
protected:

	int _Health, _Speed, _MaxHealth;

	sf::Vector2f _Movement;
};

