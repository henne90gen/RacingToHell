#pragma once

#include "stdafx.h"
#include "Car.h"
#include "AICar.h"
#include "GameObject.h"
#include "GameObjects.h"

class PlayerCar : public Car
{
public:
	PlayerCar(int HP, int Speed);
	~PlayerCar();

	//Inherited functions from GameObject
	void handleEvent(sf::Event& Event);
	void update(float FrameTime);

	bool checkForCollision(GameObject * go);
};

