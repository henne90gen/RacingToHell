#pragma once

#include "Menu.h"
#include "UserInterface/Menu/MenuItem/StatBox.h"
#include "Multiplayer/NetworkHandle.h"
#include "Multiplayer/Communication.h"

class MultiplayerLobby : public Menu
{
public:
	MultiplayerLobby(Framework &framework);
	virtual ~MultiplayerLobby() {}

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
    void handleEvent(sf::Event &event);

	void update(std::pair<NetworkCommunication, int> lastresponse);

	void setNetworkHandle(NetworkHandle *Handle) { _NetworkHandle = Handle; }

	void EnableButtons(bool isAdmin);

	int getCarIndex() { return _SelectedCar; }
	void setCarIndex(int x) { _SelectedCar = x; }

	void resetTable() { _MenuItems[(int)MenuItemIndex::PlayerTable]->resetTable(); }
	void addPlayer(std::string name, bool isadmin) { _MenuItems[(int)MenuItemIndex::PlayerTable]->addPlayer(name, isadmin); }
	void removePlayer(int index) { _MenuItems[(int)MenuItemIndex::PlayerTable]->removePlayer(index); }
	void setAdminTable(bool admin) { _MenuItems[(int)MenuItemIndex::PlayerTable]->setAdmin(admin); }
	bool getClientReady() { return _MenuItems[(int)MenuItemIndex::PlayerTable]->getPlayerReady(); }
	void setClientReady(bool rdy) { _MenuItems[(int)MenuItemIndex::PlayerTable]->setMember(1, rdy); }
	void resetRdyButton() { _MenuItems[(int)MenuItemIndex::Ready]->setText("Ready"); }

private:
	enum class MenuItemIndex {PlayerTable = 0, Start = 1, Difficulty = 2, Modes = 3, Ready = 4, PrevCar = 5, NextCar = 6};

	std::shared_ptr<StatBox> _StatBox;

	NetworkHandle *_NetworkHandle;

	int _SelectedCar;
};
