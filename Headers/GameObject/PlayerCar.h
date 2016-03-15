#pragma once

#include "stdafx.h"
#include "Car.h"
#include "AICar.h"
#include "GameObject.h"
#include "GameObject\GameObjectType.h"
#include "PlayerStats.h"

class PlayerCar : public Car
{
public:
	/*
		Car that is controlled by the player
		@param selectedCar Index of the selected car to set the stats
		@param texture Texture that is going to be used for the sprite
	*/
	PlayerCar(int selectedCar, sf::Texture& texture);
	~PlayerCar() {}

	/*
		Renders the PlayerCar to the specified RenderWindow
		@param window Window to draw to
		@param renderCrosshair Determines whether various aiming utilities (crosshair and aim line) are going to be rendered
	*/
	void render(sf::RenderWindow& window, bool renderCrosshair);
	
	/*
		Handles events for PlayerCar
		@param newEvent Event to be handled
	*/
	void handleEvent(sf::Event& newEvent);

	/*
		Updates the PlayerCar with the given frame time
		@param frameTime Time that has passed since the last update
		@param roadSpeed Speed of the road
	*/
	void update(float frameTime, int roadSpeed);

	/*
		Gives back the angle of the shot bullet or 360.0f if no bullet was fired
		@return float Angle of the shot bullet
	*/
	float shotBullet() { return _ShotBullet; }

	/*
		Resets angle for new bullets so that no new bullets are fired
	*/
	void resetShotBullet() { _ShotBullet = 360.0f; }

	/*
		Adds 20 to the players health if possible
	*/
	void addHealth();

	/*
		@return float Energy of the player
	*/
	float getEnergy() { return _Energy; }

	/*
		@return float Maximum amount of energy the player can have
	*/
	int getMaxEnergy() { return _MaxEnergy; }

	/*
		Adds 50 to the players energy if possible
	*/
	void addEnergy();

	/*
		@return int Damage of a bullet fired by the player
	*/
	int getBulletdamage() { return _Bulletdamage; }

	/*
		Changes the players stats according to the id of the car he chose.
	*/
	void setStats(int id);

	/*
		Resets the movemnt vector that is applied to the PlayerCar
	*/
	void resetMovement() { _Movement = sf::Vector2f(0, 0); }
private:
	float _ShotBullet, _Energy;
	int _MaxEnergy, _Bulletdamage;

	sf::RectangleShape _AimLine;
	sf::Vector2f _CrosshairMovement;
	float _CrosshairSpeed;
	sf::Texture _CrosshairTexture;
	sf::Sprite _Crosshair;
};

