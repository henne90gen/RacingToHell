#include "stdafx.h"
#include "Menu/AboutScreen.h"

AboutScreen::AboutScreen() : Menu(GameState::About)
{
	sf::Vector2f ButtonSize = sf::Vector2f(150, 50);
	std::shared_ptr<MenuButton> button(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 200, 735), ButtonSize, MenuResult::Back, "Back", TextAlignment::Center));
	_MenuItems.push_back(button);

	_Text.setFont(_Font);
	_Text.setColor(sf::Color::Black);
	_Text.setString("This game was made by \nAlex Ivliev and Hendrik Müller.");
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 100));

	_Music.setFont(_Font);
	_Music.setColor(sf::Color::Black);
	_Music.setString("Music by KsTBeats");
	_Music.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 200));

	_Cars.setFont(_Font);
	_Cars.setColor(sf::Color::Black);
	_Cars.setString("Enemy cars by sujit1717");
	_Cars.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 250));

	_Mech.setFont(_Font);
	_Mech.setColor(sf::Color::Black);
	_Mech.setString("Boss sprites by Skorpio, MillionthVector and Bleed");
	_Mech.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 300));

	_LevelUp.setFont(_Font);
	_LevelUp.setColor(sf::Color::Black);
	_LevelUp.setString("'Level up sound effect' by Bart Kelsey");
	_LevelUp.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 350));
}

void AboutScreen::render(sf::RenderWindow & window)
{
	window.draw(_Text);
	window.draw(_Music);
	window.draw(_Cars);
	window.draw(_LevelUp);

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
