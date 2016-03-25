#include "stdafx.h"
#include "Menu/LevelUpScreen.h"

LevelUpScreen::LevelUpScreen() : _ShowTime(2.0f), _FadeTime(0.1f)
{
	if (_Font.loadFromFile("Resources/Font/arial.ttf")) {
		_Text.setFont(_Font);
		_Text.setColor(sf::Color::White);
		_Text.setString("Prepare for stronger and faster enemies!");
		_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 50));
	}

	_FasterTexture.loadFromFile("Resources/Texture/fast_forward.png");
	_FasterTexture.setSmooth(true);
	_Faster.setTexture(_FasterTexture);
	_Faster.setScale(sf::Vector2f(0.5f, 0.5f));
	_Faster.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Faster.getLocalBounds().width / 4, 150));
	
	_StrongerTexture.loadFromFile("Resources/Texture/double_arrow.png");
	_StrongerTexture.setSmooth(true);
	_Stronger.setTexture(_StrongerTexture);
	_Stronger.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Stronger.getLocalBounds().width / 2, 450));

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
		window.draw(_Faster);
		window.draw(_Stronger);
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
