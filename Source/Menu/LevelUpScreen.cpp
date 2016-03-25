#include "stdafx.h"
#include "Menu/LevelUpScreen.h"

LevelUpScreen::LevelUpScreen() : _ShowTime(2.0f), _FadeTime(0.1f)
{
	if (_Font.loadFromFile("Resources/Font/arial.ttf")) {
		_Text.setFont(_Font);
		_Text.setColor(sf::Color::White);
		_Text.setString("Level up!\nPrepare for stronger and faster enemies");
		_Text.setPosition(sf::Vector2f(50, 50));
	}

	_Texture.loadFromFile("Resources/Texture/fast_forward.png");
	_Sprite.setTexture(_Texture);
	_Sprite.setColor(sf::Color::White);
	_Sprite.setPosition(sf::Vector2f());

	_Background.setPosition(sf::Vector2f(0, 0));
	_Background.setSize(sf::Vector2f(SCREENWIDTH, SCREENHEIGHT));

	_Timer.restart();
}

void LevelUpScreen::render(sf::RenderWindow& window) {
	float alpha = 0.0f;
	if (_FadeTimer.getElapsedTime().asSeconds() <= _FadeTime) {
		alpha = (_FadeTimer.getElapsedTime().asSeconds() * 255.0f) / _FadeTime;
	}
	else {
		alpha = 255.0f;
	}
	_Background.setFillColor(sf::Color(0, 0, 0, alpha));
	window.draw(_Background);
	if (alpha >= 200.0f) {
		window.draw(_Text);
		window.draw(_Sprite);
	}
}

bool LevelUpScreen::update() {
	if (_Timer.getElapsedTime().asSeconds() > _ShowTime) {
		_Timer.restart();
		return true;
	}
	return false;
}

void LevelUpScreen::levelUp()
{
	_Timer.restart();
	_FadeTimer.restart();
}
