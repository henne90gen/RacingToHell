#include "stdafx.h"
#include "Menu\GameOverScreen.h"

GameOverScreen::GameOverScreen() : _SoundPlayed(false)
{
	if (_Font.loadFromFile("Resources/Font/arial.ttf")) {
		_GOTLine1.setFont(_Font);
		_GOTLine1.setColor(sf::Color(200, 0, 0));
		_GOTLine1.setCharacterSize(60);
		_GOTLine1.setString("Game Over!");
		_GOTLine1.setOrigin(_GOTLine1.getLocalBounds().left + _GOTLine1.getLocalBounds().width / 2.0f, _GOTLine1.getLocalBounds().top + _GOTLine1.getLocalBounds().height / 2.0f);
		_GOTLine1.setPosition(SCREENWIDTH / 2.0f, 50);

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
		
		sf::Vector2f ButtonSize = sf::Vector2f(150, 50);

		_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2 - 225, 735), ButtonSize, MenuResult::BackToMain));
		_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2 - 225 + 450, 735), ButtonSize, MenuResult::SubmitScore));
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
	Window.draw(_GOTLine3);
	Window.draw(_GOTLine4);

	_Textbox->render(Window);
	_Highscore->render(Window);

	for (unsigned int i = 0; i < _MenuItems.size(); i++)
	{
		_MenuItems[i]->render(Window);
	}

	if (MenuItemHovered())
	{
		sf::StandardCursor Cursor(sf::StandardCursor::HAND);
		Cursor.set(Window.getSystemHandle());
	}
	else if (_Textbox->MouseOverTextbox(sf::Mouse::getPosition(Window)))
	{
		sf::StandardCursor Cursor(sf::StandardCursor::TEXT);
		Cursor.set(Window.getSystemHandle());
	}
	else
	{
		sf::StandardCursor Cursor(sf::StandardCursor::NORMAL);
		Cursor.set(Window.getSystemHandle());
	}
}

void GameOverScreen::render(sf::RenderWindow & Window)
{
}

void GameOverScreen::update()
{
	_Textbox->update();
}

void GameOverScreen::playSounds()
{
	if (_GameOverSound.getStatus() == sf::Sound::Stopped || _GameOverSound.getStatus() == sf::Sound::Paused) {
		if (!_SoundPlayed) {
			_GameOverSound.play();
			_SoundPlayed = true;
		}
	}
}

void GameOverScreen::setVolume(float Volume)
{
	_GameOverSound.setVolume(Volume + 10);
}

GameState GameOverScreen::handleEvents(sf::RenderWindow & Window)
{
	while (Window.pollEvent(_Event)) {
		checkMouseHover(Window);
		if (_Event.type == sf::Event::MouseButtonPressed)
		{
			sf::Vector2f MousePos = sf::Vector2f(sf::Mouse::getPosition(Window));
			for (int i = 0; i < getMenuItems().size(); i++) {
				sf::FloatRect rect = getMenuItems()[i]->getRect();
				if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width) {
					switch (getMenuItems()[i]->getAction()) {
					case MenuResult::BackToMain:
						_SoundPlayed = false;
						return GameState::Main;
						break;
					case MenuResult::SubmitScore:
						break;
					}
				}
			}
		}

		if (_Event.type == sf::Event::Closed) {
			return GameState::Exiting;
		}
		_Textbox->handleEvent(_Event);
	}
	return GameState::GameOver;
}

void GameOverScreen::loadHighScores()
{
	_Highscore = new Highscore(sf::Vector2f(SCREENWIDTH / 2 - 225, 225));
	if (_GameOverSoundBuffer.loadFromFile("Resources/Sound/gameOver.wav")) {
		_GameOverSound.setBuffer(_GameOverSoundBuffer);
	}
}
