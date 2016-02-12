#include "stdafx.h"
#include "Menu\GameOverScreen.h"


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

void GameOverScreen::render(sf::RenderWindow& Window, int score)
{
	_GameOverText.setString("Game Over!\nRestart with Spacebar\n\nYour score was: " + std::to_string(score));
	Window.draw(_GameOverText);
}