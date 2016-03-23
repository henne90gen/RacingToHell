#pragma once
#include "Menu/Menu.h"
#include "Menu/MenuItem/HighscoreList.h"

class HighscoreMenu : public Menu
{
public:
	/*
		Shows the HighscoreList, nothing more
	*/
	HighscoreMenu();
	~HighscoreMenu() {}

	/*
		Renders the HighscoreMenu to the specified RenderWindow
		@param window Window to draw to
	*/
	void render(sf::RenderWindow & Window);
	
	/*
		Handles events for the HighscoreMenu
		@param window RenderWindow to poll the events from
		@return GameState New GameState the game should be in
	*/
	GameState handleEvents(sf::RenderWindow & Window);
	
	/*
		Handles the result of the different MenuItems that are on the HighscoreMenu
		@param result What the MenuItems returned as a MenuResult
		@return GameState New GameState the game should be in
	*/
	GameState handleMenuItemResult(MenuResult result);

	void loadScoreTable() { _List.loadScoreTable(); }

private:
	HighscoreList _List;
};

