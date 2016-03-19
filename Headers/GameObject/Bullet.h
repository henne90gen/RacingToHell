#pragma once

#include "GameObject\GameObject.h"
#include "GameObject\GameObjectType.h"

class Bullet : public GameObject
{
public:
	/*
		Bullet that can be shot by any Car
		@param pos Starting position of the Bullet
		@param dir Angle at which the Bullet will fly
		@param speed Speed of the Bullet
		@param type Specifies the type of the Bullet, thus who shot it
		@param texture Texture that is going to be used for the sprite
	*/
	Bullet(sf::Vector2f pos, sf::Vector2f dir, int speed, GameObjectType type, sf::Texture& texture);
	~Bullet() {}

	/*
		Handles events for Bullet
		@param newEvent Event to be handled
	*/
	void handleEvent(sf::Event& newEvent) {}

	/*
		Updates the Bullet with the given frame time
		@param frameTime Time that has passed since the last update
		@param roadSpeed Speed of the road
	*/
	void update(float frameTime, int roadSpeed);
	

private:
	float _Speed;
	sf::Vector2f _Direction;
};