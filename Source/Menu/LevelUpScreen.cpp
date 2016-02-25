#include "stdafx.h"
#include "Menu/LevelUpScreen.h"


LevelUpScreen::LevelUpScreen()
{
	if (_Font.loadFromFile("Resources/Font/arial.ttf")) {
		_Text.setFont(_Font);
		_Text.setColor(sf::Color::White);
		_Text.setString("LevelUp!");
		_Text.setPosition(sf::Vector2f(50, 50));
	}
	_Timer.restart();
}


LevelUpScreen::~LevelUpScreen()
{
}

void LevelUpScreen::render(sf::RenderWindow& Window) {
	Window.clear(sf::Color::Black);
	Window.draw(_Text);
}

bool LevelUpScreen::update() {
	if (_Timer.getElapsedTime().asSeconds() > 7) {
		_Timer.restart();
		return true;
	}
	return false;
}