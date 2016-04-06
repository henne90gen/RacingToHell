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
	PlayerCar(unsigned int id, int selectedCar, sf::Texture& texture);
	PlayerCar(std::istream& stream, std::vector<std::shared_ptr<sf::Texture>>& textures);
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
	sf::Vector2f shotBullet() { return _ShotBullet; }

	/*
		Resets angle for new bullets so that no new bullets are fired
	*/
	void resetShotBullet() { _ShotBullet = sf::Vector2f(0, 0); }

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

	/*
		Player loses Energy
	*/
	void drainEnergy(float FrameTime) { _Energy -= 2 * FrameTime; }

	/*
		Fills up resources to its maximum
	*/
	void resetResources() { _Health = _MaxHealth; _Energy = _MaxEnergy; }

	void operator>>(std::ostream& stream);

	void operator<<(std::istream& stream);
private:
	float _Energy;
	sf::Uint16 _MaxEnergy, _Bulletdamage, _SelectedCar;

	sf::Vector2f _ShotBullet;

	sf::RectangleShape _AimLine;
	sf::Vector2f _CrosshairMovement;
	float _CrosshairSpeed;
	sf::Texture _CrosshairTexture;
	sf::Sprite _Crosshair;
};

