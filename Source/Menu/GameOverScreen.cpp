#include "stdafx.h"
#include "Menu\GameOverScreen.h"

GameOverScreen::GameOverScreen() : Menu(GameState::GameOver), _SoundPlayed(false), _ScoreSubmitted(false)
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
		
		sf::Vector2f ButtonSize = sf::Vector2f(150, 50);

		_MenuItems.push_back(new Textbox(sf::Vector2f(_GOTLine4.getPosition().x + _GOTLine4.getLocalBounds().width + 20, _GOTLine4.getPosition().y + 10), sf::Vector2f(450 - _GOTLine4.getLocalBounds().width - 20, _GOTLine4.getLocalBounds().height), 25, "Test", true));
		_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 + 200, 735), ButtonSize, MenuResult::SubmitScore, "Submit", TextAlignment::Center));
		_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 200, 735), ButtonSize, MenuResult::Back, "Back", TextAlignment::Center));
	}
}

GameOverScreen::~GameOverScreen()
{
	delete _Highscore;
	_Highscore = nullptr;
}

void GameOverScreen::render(sf::RenderWindow& Window)
{
	_GOTLine3.setString("Your score was: " + std::to_string(_Highscore->getScore()));

	Window.draw(_GOTLine1);
	Window.draw(_GOTLine3);
	Window.draw(_GOTLine4);

	_Highscore->render(Window);

	for (unsigned int i = 0; i < _MenuItems.size(); i++)
	{
		_MenuItems[i]->render(Window);
	}

	checkMenuItemHovered(Window);
}

void GameOverScreen::update(int Score, int Level)
{
	_Highscore->setScore(Score);
	_Level = Level;
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
	if (Volume > 0) {
		_GameOverSound.setVolume(Volume + 10);
	}
	else {
		_GameOverSound.setVolume(Volume);
	}
}

GameState GameOverScreen::handleEvents(sf::RenderWindow & Window)
{
	while (Window.pollEvent(_Event)) {
		
		float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

		if (Y < 10 && Y > -10) {
			_JoystickTimer.restart();
		}

		if (_JoystickTimer.getElapsedTime().asSeconds() >= _JoystickDelay) {
			if (Y < -80) {
				if (_JoystickSelection > 0) {
					_JoystickSelection--;
					_JoystickTimer.restart();
				}
			}
			else if (Y > 80) {
				if (_JoystickSelection < _MenuItems.size() - 1) {
					_JoystickSelection++;
					_JoystickTimer.restart();
				}
			}
		}

		return handleMenuItems(_Event);
	}
	return _MenuGameState;
}

GameState GameOverScreen::handleMenuItemResult(MenuResult result)
{
	std::string name = dynamic_cast<Textbox*>(_MenuItems[0])->getText();
	switch (result) {
	case MenuResult::Back:
		_SoundPlayed = false;
		_ScoreSubmitted = false;
		return GameState::Main;
		break;
	case MenuResult::SubmitScore:
		if (!_ScoreSubmitted && _Highscore->getScore() > _Highscore->MinScore() && name != "")
		{
			_Highscore->PlacePlayer(name, _Level);
			_Highscore->SaveScoreTable();
			_Highscore->loadScoreTable();
			_ScoreSubmitted = true;
			_MenuItems[0]->setEnabled(false);
			_MenuItems[1]->setEnabled(false);
		}
		break;
	}
	return _MenuGameState;
}

void GameOverScreen::loadHighScores()
{
	_Highscore = new Highscore(sf::Vector2f(SCREENWIDTH / 2 - 225, 225));
	if (_GameOverSoundBuffer.loadFromFile("Resources/Sound/gameOver.wav")) {
		_GameOverSound.setBuffer(_GameOverSoundBuffer);
	}
}
