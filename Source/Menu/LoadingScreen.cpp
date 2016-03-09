#include "stdafx.h"
#include "Menu/LoadingScreen.h"


LoadingScreen::LoadingScreen() : Menu(GameState::GameOver), _IsFadingAway(false), _FadeTime(1.0f)
{
	_Text.setString("Loading...");
	_Text.setCharacterSize(75);
	_Text.setOrigin(sf::Vector2f(_Text.getLocalBounds().width / 2.0f, _Text.getLocalBounds().height / 2.0f));
	_Text.setPosition(SCREENWIDTH/2, SCREENHEIGHT/2);

	_Background.setSize(sf::Vector2f(SCREENWIDTH, SCREENHEIGHT));
	_Background.setPosition(sf::Vector2f(0, 0));
	_Background.setFillColor(sf::Color(0, 0, 0));
}


LoadingScreen::~LoadingScreen()
{
}

void LoadingScreen::render(sf::RenderWindow & Window)
{
	float alpha = 255.0f;
	if (_IsFadingAway) {
		alpha = (_FadeTime - _FadeTimer.getElapsedTime().asSeconds()) * 255.0f / _FadeTime;
		_Background.setFillColor(sf::Color(_Background.getFillColor().r, _Background.getFillColor().g, _Background.getFillColor().b, alpha));
		_Text.setColor(sf::Color(_Text.getColor().r, _Text.getColor().g, _Text.getColor().b, alpha));
	}
	Window.draw(_Background);
	Window.draw(_Text);
}

GameState LoadingScreen::handleEvents(sf::RenderWindow & Window)
{
	return GameState::Loading;
}

GameState LoadingScreen::handleMenuItemResult(MenuResult result)
{
	return GameState();
}

void LoadingScreen::fadeAway()
{
	_FadeTimer.restart();
	_IsFadingAway = true;
}

bool LoadingScreen::isDoneFading()
{
	if (_FadeTimer.getElapsedTime().asSeconds() > _FadeTime) {
		return true;
	}
	return false;
}