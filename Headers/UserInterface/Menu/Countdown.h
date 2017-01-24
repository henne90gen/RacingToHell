#pragma once
#include "Menu.h"

class Countdown : public Menu
{
public:
	Countdown(Framework &framework);
	~Countdown();

	/*
		Renders the LoadingScreen to the specified RenderWindow
		@param window Window to draw to
	*/
	void render(sf::RenderWindow& window);

	/*
		Handles events for the LoadingScreen
		@param window RenderWindow to poll the events from
		@return GameState New GameState the game should be in
	*/
	void handleEvent(sf::Event &event) {}

	void update(float frameTime);

	void fastForward(float time);

private:
	sf::Clock _Timer;
	bool _Started;
	float _AdditionalTime;
	sf::RectangleShape _Background;
};

