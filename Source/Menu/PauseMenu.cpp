#include "stdafx.h"
#include "Menu/PauseMenu.h"


PauseMenu::PauseMenu()
{
	//Menu-Items
	sf::Vector2f ButtonSize = sf::Vector2f(150, 50);

	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2, 240), ButtonSize, MenuResult::Resume));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2, 310), ButtonSize, MenuResult::Option));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2, 380), ButtonSize, MenuResult::Exit));

	//Menu-Text
	_Text.setString("Pause Menu");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));
}


PauseMenu::~PauseMenu()
{
}

void PauseMenu::render(sf::RenderWindow & Window)
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}
	Window.draw(_Text);
}

GameState PauseMenu::handleEvents(sf::RenderWindow & Window)
{
	while (Window.pollEvent(_Event)) {
		checkMouseHover(Window);
		if (_Event.type == sf::Event::MouseButtonPressed) {
			sf::Vector2f MousePos = sf::Vector2f(sf::Mouse::getPosition(Window));
			for (int i = 0; i < getMenuItems().size(); i++) {
				sf::FloatRect rect = getMenuItems()[i]->getRect();
				if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width) {
					switch (getMenuItems()[i]->getAction()) {
					case MenuResult::Resume:
						return GameState::Running;
						break;
					case MenuResult::Option:
						return GameState::Options;
						break;
					case MenuResult::Nothing:
						return GameState::Pause;
						break;
					case MenuResult::Exit:
						return GameState::Exiting;
						break;
					}
				}
			}
		}
		else if (_Event.type == sf::Event::Closed) {
			return GameState::Exiting;
		}
	}
	return GameState::Pause;
}
