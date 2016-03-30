#include "stdafx.h"
#include "Menu\MultiplayerLobby.h"

MultiplayerLobby::MultiplayerLobby() : Menu(GameState::Lobby)
{
	std::shared_ptr<MenuButton> LeaveButton(new MenuButton(sf::Vector2f(100, 700), sf::Vector2f(150, 50), MenuResult::Back, "Leave", TextAlignment::Center));
	_MenuItems.push_back(LeaveButton);
}

void MultiplayerLobby::render(sf::RenderWindow& Window)
{
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
		return GameState::Main;
		break;
	default:
		break;
	}

	return _MenuGameState;
}

void MultiplayerLobby::update(float frametime)
{

}