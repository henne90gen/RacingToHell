#include "stdafx.h"
#include "Menu\GameOverScreen.h"

GameOverScreen::GameOverScreen()
{
	if (_Font.loadFromFile("Resources/Font/arial.ttf")) {
		_GOTLine1.setFont(_Font);
		_GOTLine1.setColor(sf::Color(200, 0, 0));
		_GOTLine1.setCharacterSize(60);
		_GOTLine1.setString("Game Over!");
		_GOTLine1.setOrigin(_GOTLine1.getLocalBounds().left + _GOTLine1.getLocalBounds().width / 2.0f, _GOTLine1.getLocalBounds().top + _GOTLine1.getLocalBounds().height / 2.0f);
		_GOTLine1.setPosition(SCREENWIDTH / 2.0f, 50);

		_GOTLine2.setFont(_Font);
		_GOTLine2.setColor(sf::Color(255, 255, 255));
		_GOTLine2.setCharacterSize(25);
		_GOTLine2.setString("Restart with Spacebar");
		_GOTLine2.setOrigin(_GOTLine2.getLocalBounds().left + _GOTLine2.getLocalBounds().width / 2.0f, _GOTLine2.getLocalBounds().top + _GOTLine2.getLocalBounds().height / 2.0f);
		_GOTLine2.setPosition(SCREENWIDTH / 2.0f, SCREENHEIGHT - 35);

		_GOTLine3.setFont(_Font);
		_GOTLine3.setColor(sf::Color(200, 0, 0));
		_GOTLine3.setCharacterSize(40);
		//_GOTLine3.setOrigin(_GOTLine3.getLocalBounds().left + _GOTLine3.getLocalBounds().width / 2.0f, _GOTLine3.getLocalBounds().top + _GOTLine3.getLocalBounds().height / 2.0f);
		_GOTLine3.setString("Your score was: ");
		_GOTLine3.setPosition(SCREENWIDTH / 2 - 225, _GOTLine1.getGlobalBounds().top + _GOTLine1.getLocalBounds().height * 2.0f);

		
		_GOTLine4.setFont(_Font);
		_GOTLine4.setColor(sf::Color(200, 0, 0));
		_GOTLine4.setCharacterSize(40);
		_GOTLine4.setString("Enter your name:");
		_GOTLine4.setPosition(SCREENWIDTH / 2 - 225, _GOTLine3.getGlobalBounds().top + _GOTLine3.getLocalBounds().height + 10);

		//_GOTLine4.setOrigin(_GOTLine4.getLocalBounds().left + _GOTLine4.getLocalBounds().width / 2.0f, _GOTLine4.getLocalBounds().top + _GOTLine4.getLocalBounds().height / 2.0f);
		

		_Textbox = new Textbox(sf::Vector2f(_GOTLine4.getPosition().x + _GOTLine4.getLocalBounds().width + 20, _GOTLine4.getPosition().y + 10), sf::Vector2f(450 - _GOTLine4.getLocalBounds().width - 20, _GOTLine4.getLocalBounds().height), 25, "Test");
		_Highscore = new Highscore(sf::Vector2f(SCREENWIDTH / 2 - 225, 225));
	}
}

GameOverScreen::~GameOverScreen()
{
	delete _Textbox;
	delete _Highscore;

	_Textbox = nullptr;
	_Highscore = nullptr;
}

void GameOverScreen::render(sf::RenderWindow& Window, int score)
{
	_GOTLine3.setString("Your score was: " + std::to_string(score));

	Window.draw(_GOTLine1);
	Window.draw(_GOTLine2);
	Window.draw(_GOTLine3);
	Window.draw(_GOTLine4);

	_Textbox->render(Window);
	_Highscore->render(Window);
}

void GameOverScreen::update()
{
	_Textbox->update();
}

void GameOverScreen::render(sf::RenderWindow & Window)
{
}

GameState GameOverScreen::handleEvents(sf::RenderWindow & Window)
{
	while (Window.pollEvent(_Event)) {
		if (_Event.type == sf::Event::KeyPressed) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				return GameState::Main;
			}
		}
		else if (_Event.type == sf::Event::Closed) {
			return GameState::Exiting;
		}
		_Textbox->handleEvent(_Event);
	}
	return GameState::GameOver;
}
