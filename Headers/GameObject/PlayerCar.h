#pragma once

#include "stdafx.h"
#include "Car.h"
#include "AICar.h"
#include "GameObject.h"
#include "GameObjects.h"
#include "PlayerStats.h"

class PlayerCar : public Car
{
public:
	PlayerCar(int SelectedCar, sf::Texture& texture);
	~PlayerCar();

	void handleEvent(sf::Event& Event);
	void update(float FrameTime, int RoadSpeed);

	bool checkForCollision(GameObject * go);

	float shotBullet() { return _ShotBullet; }
	void resetShotBullet() { _ShotBullet = 360.0f; }

	void takeDamage() { _Health -= 5; }
	void addHealth();
	int getMaxHealth() { return _MaxHealth; }

	float getEnergy() { return _Energy; }
	int getMaxEnergy() { return _MaxEnergy; }
	void addEnergy();

	int getBulletdamage() { return _Bulletdamage; }

	void setStats(int id);
private:
	float _ShotBullet, _Energy;
	int _MaxEnergy, _MaxHealth, _Bulletdamage;
};

