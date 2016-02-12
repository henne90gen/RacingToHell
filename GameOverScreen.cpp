#include "stdafx.h"
#include "GameOverScreen.h"


GameOverScreen::GameOverScreen()
{
	if (_Font.loadFromFile("Resources/arial.ttf")) {
		_GameOverText.setFont(_Font);
		_GameOverText.setPosition(sf::Vector2f(0,0));
		_GameOverText.setColor(sf::Color::Blue);
		_GameOverText.setCharacterSize(50);
	}
}


GameOverScreen::~GameOverScreen()
{
}

MenuResult GameOverScreen::render(sf::RenderWindow& Window, int score)
{
	_GameOverText.setString("Game Over!\nRestart with Spacebar\n\nYour score was: " + std::to_string(score));
	Window.draw(_GameOverText);
	Window.display();
	return getMenuResponse(Window);
}

MenuResult GameOverScreen::getMenuResponse(sf::RenderWindow& Window) {
	sf::Event event;
	while (Window.pollEvent(event)) {
		if (event.type == sf::Event::MouseButtonPressed) {
			return handleClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
		}
		else if (event.type == sf::Event::KeyPressed) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			return MenuResult::Restart;
			}
		}
		else if (event.type == sf::Event::Closed) {
			return MenuResult::Exit;
		}
	}
}

MenuResult GameOverScreen::handleClick(sf::Vector2f MousePos)
{
	return MenuResult::Nothing;
}
