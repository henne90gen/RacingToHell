#pragma once
#include "Menu/Menu.h"

class Countdown : public Menu
{
public:
	Countdown();
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
	GameState handleEvents(sf::RenderWindow& window);

	/*
		Handles the result of the different MenuItems that are on the LoadingScreen
		@param result What the MenuItems returned as a MenuResult
		@return GameState New GameState the game should be in
	*/
	GameState handleMenuItemResult(MenuResult result);

	bool update(float frameTime);

private:
	sf::Clock _Timer;
	bool _Started;

	sf::RectangleShape _Background;
};

