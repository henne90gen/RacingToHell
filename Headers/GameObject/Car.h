#pragma once
#include "stdafx.h"
#include "GameObject\GameObject.h"
#include "GameObject\GameObjectType.h"

#include <string>

class Car : public GameObject
{
public:
	/*
		Car that can drive around on the road
		@param pos Initial position of the car
		@param maxHealth Maximum health the car can have
		@param speed Speed of the Car
		@param type Type of the Car
		@param texture Texture that is going to be used for the sprite
	*/
	Car(sf::Vector2f pos, int maxHealth, float speed, GameObjectType type, sf::Texture& texture);
	~Car() {}

	/*
		Handles events for Car
		@param newEvent Event to be handled
	*/
	virtual void handleEvent(sf::Event& newEvent) {}

	/*
		Updates the Car with the given frame time
		@param frameTime Time that has passed since the last update
		@param roadSpeed Velocity of the road
	*/
	virtual void update(float frameTime, int roadSpeed) {}

	/*
		@return int Health of the Car
	*/
	int getHealth();

	/*
		@return int Maximum health of the Car
	*/
	int getMaxHealth() { return _MaxHealth; }

	/*
		@return int Speed of the Car
	*/
	int getSpeed() { return _Speed; }

	/*
		@param health Set the health of the Car
	*/
	void setHealth(int health) { _Health = health; }

	/*
		@param speed Set the speed of the Car
	*/
	void setSpeed(int speed) { _Speed = speed; }

	/*
		Damage the car and subtract the damage from the health
		@param damage Amount of health to subtract
	*/
	void takeDamage(int damage) { _Health -= damage; }

protected:
	int _Health, _Speed, _MaxHealth;

	sf::Vector2f _Movement;
};

