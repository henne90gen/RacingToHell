#pragma once

#include "stdafx.h"
#include "Car.h"
#include "AICar.h"
#include "GameObject.h"
#include "GameObjects.h"

class PlayerCar : public Car
{
public:
	PlayerCar();
	~PlayerCar();

	void handleEvent(sf::Event& Event);
	void update(float FrameTime);

	bool checkForCollision(GameObject * go);

	float shotBullet() { return _ShotBullet; };
	void resetShotBullet() { _ShotBullet = 360.0f; }

	void takeDamage() { _Health -= 5; }
	void addHealth();
	int getMaxHealth() { return _MaxHealth; }

	int getEnergy() { return _Energy; }
	int getMaxEnergy() { return _MaxEnergy; }
	void addEnergy();

private:
	float _ShotBullet;
	int _Energy, _MaxEnergy, _MaxHealth;
};

