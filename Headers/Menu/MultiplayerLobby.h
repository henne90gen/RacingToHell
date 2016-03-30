#pragma once

#include "Menu.h"
#include "MenuItem\StatBox.h"

class MultiplayerLobby : public Menu
{
public:
	MultiplayerLobby();
	~MultiplayerLobby() {}

	/*
	Renders the OptionsMenu to the specified RenderWindow
	@param window Window to draw to
	*/
	void render(sf::RenderWindow& Window);

	/*
	Handles events for the OptionsMenu
	@param window RenderWindow to poll the events from
	@return GameState New GameState the game should be in
	*/
	GameState handleEvents(sf::RenderWindow& Window);

	/*
	Handles the result of the different MenuItems that are on the OptionsMenu
	@param result What the MenuItems returned as a MenuResult
	@return GameState New GameState the game should be in
	*/
	GameState handleMenuItemResult(MenuResult result);

	void update(float frametime);
private:
	sf::RectangleShape _TablePlaceHolder;

	std::shared_ptr<StatBox> _StatBox;

	int _SelectedCar;
};