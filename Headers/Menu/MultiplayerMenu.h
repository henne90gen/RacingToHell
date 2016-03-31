#pragma once

#include "Menu.h"
#include "Communication.h"
#include "Multiplayer\NetworkHandle.h"

#include <thread>
#include <future>

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

	NetworkCommunication update(float frametime);

	bool getCreatedLobby() { return _CreatedLobby; }

	void setNetWorkHandle(NetworkHandle *Handle) { _NetworkHandle = Handle; }
private:
	sf::RectangleShape _BackgroundJoin, _BackgroundName, _BackgroundCreate;
	sf::Text _NameText, _JoinText, _IPText, _PasswordText, _FeedbackText, _CreateText, _CreatePassword;

	int _CreatedLobby;
	NetworkHandle *_NetworkHandle;

	enum class MenuItemIndex {Name = 0, IP = 1, Port = 2, PasswordJoin = 3, PasswordCreate = 5};

	std::thread _ConnectionThread;
	std::mutex _Mutex;
};