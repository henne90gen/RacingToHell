#include "stdafx.h"
#include "Menu\GameOverScreen.h"

GameOverScreen::GameOverScreen() : _SoundPlayed(false), _ScoreSubmitted(false)
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

void GameOverScreen::render(sf::RenderWindow& Window)
{
	_GOTLine3.setString("Your score was: " + std::to_string(_Highscore->getScore()));

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
		if (!_Textbox->getDisabled())
		{
			sf::StandardCursor Cursor(sf::StandardCursor::TEXT);
			Cursor.set(Window.getSystemHandle());
		}
	}
	else
	{
		sf::StandardCursor Cursor(sf::StandardCursor::NORMAL);
		Cursor.set(Window.getSystemHandle());
	}
}

void GameOverScreen::update(int Score, int Level)
{
	_Highscore->setScore(Score);
	_Level = Level;
	_Textbox->setDisabled(!_Highscore->isNewHighscore() || _ScoreSubmitted);
	_MenuItems[1]->setEnabled(!_Textbox->getDisabled());
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
		if (_Event.type == sf::Event::Closed) {
			return GameState::Exiting;
		} 
		else if (_Event.type == sf::Event::MouseButtonPressed || _Event.type == sf::Event::JoystickButtonPressed) {
			sf::Vector2f MousePos = sf::Vector2f(sf::Mouse::getPosition(Window));
			for (int i = 0; i < getMenuItems().size(); i++) {
				sf::FloatRect rect = getMenuItems()[i]->getRect();
				if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width ||
					sf::Joystick::isButtonPressed(0, 0)) 
				{
					if (sf::Joystick::isButtonPressed(0, 0)) {
						i = _JoystickSelection;
					}
					switch (getMenuItems()[i]->getAction()) {
					case MenuResult::BackToMain:
						_SoundPlayed = false;
						_ScoreSubmitted = false;
						return GameState::Main;
						break;
					case MenuResult::SubmitScore:
						if (!_ScoreSubmitted && _Highscore->getScore() > _Highscore->MinScore() && _Textbox->getText() != "")
						{
							_Highscore->PlacePlayer(_Textbox->getText(), _Level);
							_Highscore->SaveScoreTable();
							_Highscore->loadScoreTable();
							_ScoreSubmitted = true;
							getMenuItems().at(i)->setEnabled(false);
						}
						break;
					}
				}
			}
		}
		else if (_Event.type == sf::Event::JoystickMoved && _JoystickTimer.getElapsedTime().asSeconds() >= _JoystickDelay) {
			float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
			float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
			_MenuItems[_JoystickSelection]->switchHoverState(false, false);
			if (Y < -80) {
				if (_JoystickSelection > 0) {
					_JoystickSelection--;
				}
			}
			else if (Y > 80) {
				if (_JoystickSelection < _MenuItems.size() - 1) {
					_JoystickSelection++;
				}
			}
			_MenuItems[_JoystickSelection]->switchHoverState(true, true);
			_JoystickTimer.restart();
		}
		else if (_Event.type == sf::Event::MouseMoved) {
			_MenuItems[_JoystickSelection]->switchHoverState(false, false);
		}
		else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < 10 && sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > -10) {
			_JoystickTimer.restart();
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
