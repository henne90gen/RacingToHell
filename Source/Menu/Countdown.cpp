#include "stdafx.h"
#include "Menu/Countdown.h"

Countdown::Countdown() : 
	Menu(GameState::Countdown),
	_AdditionalTime(0.0f)
{
	_Text.setString("4");
	_Text.setCharacterSize(75);
	_Text.setOrigin(sf::Vector2f(_Text.getLocalBounds().width / 2.0f, _Text.getLocalBounds().height / 2.0f));
	_Text.setPosition(SCREENWIDTH / 2, SCREENHEIGHT / 2);

	_Background.setSize(sf::Vector2f(SCREENWIDTH, SCREENHEIGHT));
	_Background.setPosition(sf::Vector2f(0, 0));
	_Background.setFillColor(sf::Color(0, 0, 0));
}


Countdown::~Countdown()
{
}

void Countdown::render(sf::RenderWindow & window)
{
	window.draw(_Background);
	window.draw(_Text);
}

GameState Countdown::handleEvents(sf::RenderWindow & window)
{
	if (window.pollEvent(_Event)) {
		return handleMenuItems(_Event);
	}
	return _MenuGameState;
}

GameState Countdown::handleMenuItemResult(MenuResult result)
{
	return _MenuGameState;
}

bool Countdown::update(float frameTime)
{
	if (!_Started) {
		_Timer.restart();
		_Started = true;
	}

	_Text.setString(std::to_string((int)(4 - (_Timer.getElapsedTime().asSeconds() + _AdditionalTime))));
	
	return (_Timer.getElapsedTime().asSeconds() + _AdditionalTime > 3);
}

void Countdown::fastForward(float time)
{
	_AdditionalTime = time;
	std::cout << "Adding " << time << " seconds." << std::endl;
}
