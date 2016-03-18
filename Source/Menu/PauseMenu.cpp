#include "stdafx.h"
#include "Menu/PauseMenu.h"


PauseMenu::PauseMenu() : Menu(GameState::Pause)
{
	//Menu-Items
	sf::Vector2f ButtonSize = sf::Vector2f(250, 50);

	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 240), ButtonSize, MenuResult::Resume, "Resume", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 310), ButtonSize, MenuResult::Option, "Options", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 380), ButtonSize, MenuResult::Back, "Main Menu", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 450), ButtonSize, MenuResult::Exit, "Exit", TextAlignment::Center));

	//Menu-Text
	_Text.setString("Pause Menu");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));
}

void PauseMenu::render(sf::RenderWindow & Window)
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}
	Window.draw(_Text);

	checkMenuItemHovered(Window);
}

GameState PauseMenu::handleEvents(sf::RenderWindow & Window)
{
	while (Window.pollEvent(_Event)) {
		
		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

		if (_Event.type == sf::Event::JoystickButtonPressed) {
			if (sf::Joystick::isButtonPressed(0, 1)) {
				return GameState::Running;
			}
		}
		
		handleJoystick(Y);

		return handleMenuItems(_Event);
	}
	return _MenuGameState;
}

GameState PauseMenu::handleMenuItemResult(MenuResult result)
{
	switch (result) {
	case MenuResult::Resume:
		return GameState::Running;
		break;
	case MenuResult::Option:
		return GameState::Options;
		break;
	case MenuResult::Back:
		return GameState::Main;
		break;
	case MenuResult::Exit:
		return GameState::Exiting;
		break;
	}
	return _MenuGameState;
}
