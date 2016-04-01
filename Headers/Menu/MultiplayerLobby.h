#pragma once

#include "Menu.h"
#include "MenuItem\StatBox.h"
#include "Multiplayer\NetworkHandle.h"
#include "Multiplayer\Communication.h"

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

	void resetTable() { _MenuItems[(int)MenuItemIndex::PlayerTable]->resetTable(); }
	void addPlayer(std::string name, bool isadmin) { _MenuItems[(int)MenuItemIndex::PlayerTable]->addPlayer(name, isadmin); }
private:
	enum class MenuItemIndex {PlayerTable = 0, Start = 1, Difficulty = 2, Modes = 3, Ready = 4};

	std::shared_ptr<StatBox> _StatBox;

	NetworkHandle *_NetworkHandle;

	int _SelectedCar;
};