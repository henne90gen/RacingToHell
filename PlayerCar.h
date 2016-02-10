#pragma once

#include "stdafx.h"
#include "Car.h"

class PlayerCar : public Car
{
public:
	PlayerCar();
	~PlayerCar();


	//Getters and Setters
	sf::Vector2f getPos() { return  _Sprite.getPosition(); }
	void setPos(sf::Vector2f Pos) { _Sprite.setPosition(Pos); }
	float getWidth() { return _Sprite.getLocalBounds().width; }
	float getHeight() { return _Sprite.getLocalBounds().height; }
	int  getHealth() { return _Health; }
	void setHealth(int Health) { _Health = Health; }

	//Inherited functions from GameObject
	void handleEvent(sf::Event& Event);
	void update(float FrameTime);
};

