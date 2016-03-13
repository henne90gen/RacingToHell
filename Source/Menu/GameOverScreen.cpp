#include "stdafx.h"
#include "Menu\GameOverScreen.h"

GameOverScreen::GameOverScreen() : Menu(GameState::GameOver), _SoundPlayed(false), _ScoreSubmitted(false), _Highscore(sf::Vector2f(SCREENWIDTH / 2 - 225, 190))
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
		_GOTLine3.setString("Your score was: ");
		_GOTLine3.setPosition(SCREENWIDTH / 2 - 225, _GOTLine1.getGlobalBounds().top + _GOTLine1.getLocalBounds().height * 1.2f);

		_GOTLine4.setFont(_Font);
		_GOTLine4.setColor(sf::Color(200, 0, 0));
		_GOTLine4.setCharacterSize(40);
		_GOTLine4.setString("Enter your name:");
		_GOTLine4.setPosition(_GOTLine3.getPosition().x, _GOTLine3.getGlobalBounds().top + _GOTLine3.getLocalBounds().height + 10);
	}
		
	sf::Vector2f ButtonSize = sf::Vector2f(150, 50);

	_MenuItems.push_back(new Textbox(sf::Vector2f(_GOTLine4.getPosition().x + _GOTLine4.getLocalBounds().width + 20, _GOTLine4.getPosition().y + 10), sf::Vector2f(450 - _GOTLine4.getLocalBounds().width - 20, _GOTLine4.getLocalBounds().height), 25, "Test", true));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 + 200, 735), ButtonSize, MenuResult::SubmitScore, "Submit", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 200, 735), ButtonSize, MenuResult::Back, "Back", TextAlignment::Center));

	_JoystickSelection = 1;
}

GameOverScreen::~GameOverScreen()
{
}

void GameOverScreen::render(sf::RenderWindow& Window)
{
	_GOTLine3.setString("Your score was: " + std::to_string(_Highscore.getScore()));

	Window.draw(_GOTLine1);
	Window.draw(_GOTLine3);
	Window.draw(_GOTLine4);

	_Highscore.render(Window);

	for (unsigned int i = 0; i < _MenuItems.size(); i++)
	{
		_MenuItems[i]->render(Window);
	}

	checkMenuItemHovered(Window);
}

void GameOverScreen::update(int Score, int Level)
{
	_Highscore.setScore(Score);
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

		handleJoystick(X);

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
		_MenuItems[0]->setEnabled(true);
		_MenuItems[1]->setEnabled(true);
		return GameState::Main;
		break;
	case MenuResult::SubmitScore:
		if (!_ScoreSubmitted && name != "")
		{
			_Highscore.PlacePlayer(name, _Level);
			_ScoreSubmitted = true;
			_MenuItems[0]->setEnabled(false);
			_MenuItems[1]->setEnabled(false);
			_JoystickSelection = 2;
		}
		break;
	}
	return _MenuGameState;
}

void GameOverScreen::load()
{
	if (_GameOverSoundBuffer.loadFromFile("Resources/Sound/gameOver.wav")) {
		_GameOverSound.setBuffer(_GameOverSoundBuffer);
	}
}
