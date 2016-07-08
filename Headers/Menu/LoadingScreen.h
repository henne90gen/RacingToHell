#pragma once
#include "Menu/Menu.h"

class LoadingScreen : public Menu
{
public:
	/*
		
	*/
	LoadingScreen();
	~LoadingScreen() {}

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

	/*
		@return bool True if the LoadingScreen is fading away
	*/
	bool isFadingAway() { return _IsFadingAway; }

	/*
		Starts the fading of the LoadingScreen
	*/
	void fadeAway();

	/*
		@return bool True when the LoadingScreen has faded away
	*/
	bool isDoneFading();

private:
	bool _IsFadingAway;
	sf::Clock _FadeTimer;
	float _FadeTime;

	sf::RectangleShape _Background;
};

