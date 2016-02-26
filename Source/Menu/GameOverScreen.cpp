#include "stdafx.h"
#include "Menu\GameOverScreen.h"

GameOverScreen::GameOverScreen()
{
	if (_Font.loadFromFile("Resources/Font/arial.ttf")) {
		_GOTLine1.setFont(_Font);
		_GOTLine1.setColor(sf::Color::Red);
		_GOTLine1.setCharacterSize(50);
		_GOTLine1.setString("Game Over!");
		_GOTLine1.setOrigin(_GOTLine1.getLocalBounds().left + _GOTLine1.getLocalBounds().width / 2.0f, _GOTLine1.getLocalBounds().top + _GOTLine1.getLocalBounds().height / 2.0f);
		_GOTLine1.setPosition(SCREENWIDTH / 2.0f, SCREENHEIGHT / 5.0f);

		_GOTLine2.setFont(_Font);
		_GOTLine2.setColor(sf::Color::Red);
		_GOTLine2.setCharacterSize(50);
		_GOTLine2.setString("Restart with Spacebar");
		_GOTLine2.setOrigin(_GOTLine2.getLocalBounds().left + _GOTLine2.getLocalBounds().width / 2.0f, _GOTLine2.getLocalBounds().top + _GOTLine2.getLocalBounds().height / 2.0f);
		_GOTLine2.setPosition(SCREENWIDTH / 2.0f, _GOTLine1.getGlobalBounds().top + _GOTLine1.getLocalBounds().height * 2.0f);

		_GOTLine3.setFont(_Font);
		_GOTLine3.setColor(sf::Color::Red);
		_GOTLine3.setCharacterSize(50);

		_Textbox = new Textbox(sf::Vector2f(20, 20), sf::Vector2f(100, 25), 20, "Test");
	}
}

GameOverScreen::~GameOverScreen()
{
	delete _Textbox;
	_Textbox = nullptr;
}

void GameOverScreen::render(sf::RenderWindow& Window, int score)
{
	_GOTLine3.setString("Your score was: " + std::to_string(score));
	_GOTLine3.setOrigin(_GOTLine3.getLocalBounds().left + _GOTLine3.getLocalBounds().width / 2.0f, _GOTLine3.getLocalBounds().top + _GOTLine3.getLocalBounds().height / 2.0f);
	_GOTLine3.setPosition(SCREENWIDTH / 2.0f, _GOTLine2.getGlobalBounds().top + _GOTLine2.getLocalBounds().height * 2.0f);

	Window.draw(_GOTLine1);
	Window.draw(_GOTLine2);
	Window.draw(_GOTLine3);

	_Textbox->render(Window);
}

void GameOverScreen::update()
{
	_Textbox->update();
}

void GameOverScreen::handleEvent(sf::Event& Event)
{
	_Textbox->handleEvent(Event);
}