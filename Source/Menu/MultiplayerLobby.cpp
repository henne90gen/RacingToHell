#include "stdafx.h"
#include "Menu\MultiplayerLobby.h"

MultiplayerLobby::MultiplayerLobby() : Menu(GameState::Lobby), _SelectedCar(0)
{
	//_LobbyMemberList = std::make_shared<PlayerTable>(sf::Vector2f(25, 25));

	std::shared_ptr<PlayerTable> Table(new PlayerTable(sf::Vector2f(25, 25)));
	_MenuItems.push_back(Table);

	std::shared_ptr<MenuButton> StartButton(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 165), sf::Vector2f(200, 50), MenuResult::Nothing, "Start", TextAlignment::Center));
	_MenuItems.push_back(StartButton);

	std::vector<std::string> difficulties = { "Easy", "Normal", "Hard", "Insane" };
	std::shared_ptr<ComboBox> comboboxDifficulty(new ComboBox(sf::Vector2f(SCREENWIDTH / 2 - 100, 235), difficulties, MenuResult::Nothing));
	_MenuItems.push_back(comboboxDifficulty);

	std::vector<std::string> modes = { "Standard" };
	std::shared_ptr<ComboBox> comboboxMode(new ComboBox(sf::Vector2f(SCREENWIDTH / 2 - 100, 305), modes, MenuResult::Nothing));
	_MenuItems.push_back(comboboxMode);

	std::shared_ptr<MenuButton> ReadyButton(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 165), sf::Vector2f(200, 50), MenuResult::Ready, "Ready", TextAlignment::Center));
	_MenuItems.push_back(ReadyButton);

	std::shared_ptr<MenuButton> buttonprev(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 60, SCREENHEIGHT - 335), sf::Vector2f(0, 50), MenuResult::PreviousSkin, "<<", TextAlignment::Left));
	_MenuItems.push_back(buttonprev);
	std::shared_ptr<MenuButton> buttonnext(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 + 50, SCREENHEIGHT - 335), sf::Vector2f(0, 50), MenuResult::NextSkin, ">>", TextAlignment::Left));
	_MenuItems.push_back(buttonnext);

	_StatBox = std::make_shared<StatBox>(sf::Vector2f(SCREENWIDTH / 2, 575));

	std::shared_ptr<MenuButton> LeaveButton(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 375), sf::Vector2f(200, 50), MenuResult::Back, "Leave", TextAlignment::Center));
	_MenuItems.push_back(LeaveButton);
}

void MultiplayerLobby::render(sf::RenderWindow& Window)
{
	_StatBox->render(Window, _SelectedCar);
	//_LobbyMemberList->render(Window);

	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}
}

GameState MultiplayerLobby::handleEvents(sf::RenderWindow& Window)
{
	checkMenuItemHovered(Window);

	if (Window.pollEvent(_Event))
	{
		return handleMenuItems(_Event);
	}

	return _MenuGameState;
}

GameState MultiplayerLobby::handleMenuItemResult(MenuResult result)
{
	switch (result)
	{
	case MenuResult::Back:
	{
		sf::Packet EmptyPacket;
		_NetworkHandle->addPacket(NetworkCommunication::Disconnect, EmptyPacket);
		std::cout << "Sending diconnect" << std::endl;
		return GameState::Main;
	}
		break;
	case MenuResult::PreviousSkin:
		_SelectedCar--;
		break;
	case MenuResult::NextSkin:
		_SelectedCar++;
		break;
	default:
		break;
	}

	return _MenuGameState;
}

void MultiplayerLobby::update(float frametime)
{

}

void MultiplayerLobby::EnableButtons(bool isAdmin)
{
	if (isAdmin)
	{
		_MenuItems[(int)MenuItemIndex::Start]->setVisible(true);
		_MenuItems[(int)MenuItemIndex::Difficulty]->setEnabled(true);
		_MenuItems[(int)MenuItemIndex::Modes]->setEnabled(true);
		_MenuItems[(int)MenuItemIndex::Ready]->setVisible(false);
	}
	else
	{
		_MenuItems[(int)MenuItemIndex::Start]->setVisible(false);
		_MenuItems[(int)MenuItemIndex::Difficulty]->setEnabled(false);
		_MenuItems[(int)MenuItemIndex::Modes]->setEnabled(false);
		_MenuItems[(int)MenuItemIndex::Ready]->setVisible(true);
	}
}
