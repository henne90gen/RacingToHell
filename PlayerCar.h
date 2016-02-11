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
	float shotBullet() { return _ShotBullet; };

	void takeDamage() { _Health -= 5; };
	void resetShotBullet() { _ShotBullet = 360.0f; }

	int getEnergy() { return _Energy; }
	void addEnergy() { _Energy += 50; }
private:
	float _ShotBullet;
	int _Energy;
};

