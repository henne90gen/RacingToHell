#pragma once

#include "stdafx.h"
#include "Car.h"

class PlayerCar : public Car
{
public:
	PlayerCar();
	~PlayerCar();


	//Getters and Setters
	

	//Inherited functions from GameObject
	void handleEvent(sf::Event& Event);
	void update(float FrameTime);
};

