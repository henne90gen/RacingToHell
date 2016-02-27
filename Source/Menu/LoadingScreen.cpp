#include "stdafx.h"
#include "Menu/LoadingScreen.h"


LoadingScreen::LoadingScreen()
{
	_Text.setString("Loading...");
	_Text.setCharacterSize(75);
	_Text.setOrigin(sf::Vector2f(_Text.getLocalBounds().width / 2.0f, _Text.getLocalBounds().height / 2.0f));
	_Text.setPosition(SCREENWIDTH/2, SCREENHEIGHT/2);
}


LoadingScreen::~LoadingScreen()
{
}

void LoadingScreen::render(sf::RenderWindow & Window)
{
	Window.draw(_Text);
}

GameState LoadingScreen::handleEvents(sf::RenderWindow & Window)
{
	return GameState::Loading;
}
