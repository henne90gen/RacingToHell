#pragma once

#include "Menu.h"

class MultiplayerMenu : Menu
{
public:
	MultiplayerMenu();
	~MultiplayerMenu() {}

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

	bool getCreatedLobby() { return _CreatedLobby; }
private:
	sf::RectangleShape _BackgroundJoin, _BackgroundName, _BackgroundCreate;
	sf::Text _NameText, _JoinText, _IPText, _PasswordText, _FeedbackText, _CreateText, _CreatePassword;

	int _CreatedLobby;
};