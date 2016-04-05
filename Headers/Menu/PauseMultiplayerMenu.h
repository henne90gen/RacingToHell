#pragma once
#include "Menu/Menu.h"

class PauseMultiplayerMenu : public Menu
{
public:
	PauseMultiplayerMenu();
	~PauseMultiplayerMenu();

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

	void setReturnState(GameState returnState) { _ReturnState = returnState; }
	GameState getReturnState() { return _ReturnState; }

private:
	GameState _ReturnState;
	sf::RectangleShape _Background;
};

