#include "stdafx.h"
#include "Menu/AboutScreen.h"


AboutScreen::AboutScreen() : Menu(GameState::About)
{
	sf::Vector2f ButtonSize = sf::Vector2f(150, 50);
	std::shared_ptr<MenuButton> button(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 200, 735), ButtonSize, MenuResult::Back, "Back", TextAlignment::Center));
	_MenuItems.push_back(button);
}

void AboutScreen::render(sf::RenderWindow & window)
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(window);
	}

	checkMenuItemHovered(window);
}

GameState AboutScreen::handleEvents(sf::RenderWindow & window)
{
	while (window.pollEvent(_Event)) {
		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

		handleJoystick(Y);

		return handleMenuItems(_Event);
	}
	return _MenuGameState;
}

GameState AboutScreen::handleMenuItemResult(MenuResult result)
{
	switch (result) {
	case MenuResult::Back:
		return GameState::Options;
		break;
	}
	return _MenuGameState;
}
