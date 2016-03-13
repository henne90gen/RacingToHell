#include "stdafx.h"
#include "Menu/HighscoreMenu.h"


HighscoreMenu::HighscoreMenu() : Menu(GameState::Highscores), _List(sf::Vector2f(100, 100))
{
	sf::Vector2f ButtonSize = sf::Vector2f(150, 50);

	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 200, 735), ButtonSize, MenuResult::Back, "Back", TextAlignment::Center));

	_Text.setString("Highscores");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));
}


HighscoreMenu::~HighscoreMenu()
{
}

void HighscoreMenu::render(sf::RenderWindow & Window)
{
	_List.render(Window);
	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}
}

GameState HighscoreMenu::handleEvents(sf::RenderWindow & Window)
{
	while (Window.pollEvent(_Event)) {

		return handleMenuItems(_Event);
	}
	return _MenuGameState;
}

GameState HighscoreMenu::handleMenuItemResult(MenuResult result)
{
	switch (result) {
	case MenuResult::Back:
		return GameState::Main;
		break;
	}
	return _MenuGameState;
}
