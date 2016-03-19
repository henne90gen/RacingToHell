#pragma once
#include "Menu\Menu.h"

class PauseMenu : public Menu
{
public:
	/*
		Whenever one pauses the game this menu appears. One can resume playing, go to options or exit.
	*/
	PauseMenu();
	~PauseMenu() {}

	/*
		Renders the PauseMenu to the specified RenderWindow
		@param window Window to draw to
	*/
	void render(sf::RenderWindow& Window);

	/*
		Handles events for the PauseMenu
		@param window RenderWindow to poll the events from
		@return GameState New GameState the game should be in
	*/
	GameState handleEvents(sf::RenderWindow& Window);

	/*
		Handles events for the PauseMenu
		@param window RenderWindow to poll the events from
		@return GameState New GameState the game should be in
	*/
	GameState handleMenuItemResult(MenuResult result);
};

