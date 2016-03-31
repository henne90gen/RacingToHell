#pragma once

#include "Car.h"
#include "GameObject\GameObjectType.h"

class AICar : public Car
{
public:
	/*
		Car that is controlled by the computer.
		@param hp Helath of the car
		@param roadSpeed Speed of the road
		@param texture Texture that is going to be used for the sprite
	*/
	AICar(int hp, int roadSpeed, sf::Texture& texture);
	AICar(std::istream& stream, sf::Texture& texture);
	~AICar() {}

	/*
		Renders the AICar to the specified RenderWindow
		@param window Window to draw to
	*/
	void render(sf::RenderWindow& window);

	/*
		Handles events for AICar
		@param newEvent Event to be handled
	*/
	void handleEvent(sf::Event& newEvent) {}

	/*
		Updates the AICar with the given frame time
		@param frameTime Time that has passed since the last update
		@param roadSpeed Speed of the road
	*/
	void update(float frameTime, int roadSpeed);
	
	/*
		Current lane the car is on
		@return int Number of the lane
	*/
	int getLane() { return _Lane; }

	void operator>>(std::ostream& stream);

	void operator<<(std::istream& stream);
private:
	sf::RectangleShape _HealthBar;
	sf::RectangleShape _HealthBarFrame;

	int _Lane;

	void init();
};

