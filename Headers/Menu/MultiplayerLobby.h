#pragma once

#include "Menu.h"
#include "MenuItem\StatBox.h"
#include "Multiplayer\NetworkHandle.h"
#include "Communication.h"

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

	void setNetworkHandle(NetworkHandle *Handle) { _NetworkHandle = Handle; }

	void EnableButtons(bool isAdmin);

	int getCarIndex() { return _SelectedCar; }
	void setCarIndex(int x) { _SelectedCar = x; }
private:
	enum class MenuItemIndex {Start = 0, Difficulty = 1, Modes = 2, Ready = 3};

	std::shared_ptr<PlayerTable> _LobbyMemberList;
	std::shared_ptr<StatBox> _StatBox;

	NetworkHandle *_NetworkHandle;

	int _SelectedCar;
};