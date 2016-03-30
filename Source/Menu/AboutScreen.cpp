#include "stdafx.h"
#include "Menu/AboutScreen.h"

AboutScreen::AboutScreen() : Menu(GameState::About)
{
	sf::Vector2f ButtonSize = sf::Vector2f(150, 50);
	std::shared_ptr<MenuButton> button(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 200, 735), ButtonSize, MenuResult::Back, "Back", TextAlignment::Center));
	_MenuItems.push_back(button);

	_Frame.setPosition(sf::Vector2f(50, 75));
	_Frame.setSize(sf::Vector2f(SCREENWIDTH - 100, 475));
	_Frame.setFillColor(sf::Color(0, 0, 0, 175));
	_Frame.setOutlineThickness(1);
	_Frame.setOutlineColor(sf::Color::Black);

	_Text.setFont(_Font);
	_Text.setStyle(sf::Text::Bold);
	_Text.setColor(sf::Color::White);
	_Text.setString("This game was made by");
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 110));

	_Text2.setFont(_Font);
	_Text2.setStyle(sf::Text::Bold);
	_Text2.setColor(sf::Color::White);
	_Text2.setString("Alex Ivliev and Hendrik Müller");
	_Text2.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text2.getLocalBounds().width / 2, 145));

	_Music.setFont(_Font);
	_Music.setCharacterSize(25);
	_Music.setColor(sf::Color::White);
	_Music.setString("Music by KsTBeats");
	_Music.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Music.getLocalBounds().width / 2, 250));

	_Cars.setFont(_Font);
	_Cars.setCharacterSize(25);
	_Cars.setColor(sf::Color::White);
	_Cars.setString("Enemy cars by sujit1717");
	_Cars.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Cars.getLocalBounds().width / 2, 300));

	_Boss.setFont(_Font);
	_Boss.setCharacterSize(25);
	_Boss.setColor(sf::Color::White);
	_Boss.setString("Boss sprites by Skorpio,");
	_Boss.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Boss.getLocalBounds().width / 2, 350));
	
	_Boss2.setFont(_Font);
	_Boss2.setCharacterSize(25);
	_Boss2.setColor(sf::Color::White);
	_Boss2.setString("MillionthVector and Bleed");
	_Boss2.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Boss2.getLocalBounds().width / 2, 385));

	_LevelUp.setFont(_Font);
	_LevelUp.setCharacterSize(25);
	_LevelUp.setColor(sf::Color::White);
	_LevelUp.setString("'Level up sound effect'");
	_LevelUp.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _LevelUp.getLocalBounds().width / 2, 435));
	
	_LevelUp2.setFont(_Font);
	_LevelUp2.setCharacterSize(25);
	_LevelUp2.setColor(sf::Color::White);
	_LevelUp2.setString("by Bart Kelsey");
	_LevelUp2.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _LevelUp2.getLocalBounds().width / 2, 470));
}

void AboutScreen::render(sf::RenderWindow & window)
{
	window.draw(_Frame);
	window.draw(_Text);
	window.draw(_Text2);
	window.draw(_Music);
	window.draw(_Cars);
	window.draw(_Boss);
	window.draw(_Boss2);
	window.draw(_LevelUp);
	window.draw(_LevelUp2);

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
