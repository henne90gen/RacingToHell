#include "stdafx.h"
#include "Screen/Menu/GameOverMenu.h"
#include "Framework/Framework.h"
GameOverMenu::GameOverMenu(Framework &framework) : Menu(framework, GameState::GameOver), _SoundPlayed(false),
                                                   _ScoreSubmitted(false),
                                                   _Highscore(sf::Vector2f(SCREENWIDTH / 2 - 225, 190)) {
    if (_Font.loadFromFile("Resources/Font/arial.ttf")) {
        _GOTLine1.setFont(_Font);
        _GOTLine1.setColor(sf::Color(200, 0, 0));
        _GOTLine1.setCharacterSize(60);
        _GOTLine1.setString("Game Over!");
        _GOTLine1.setOrigin(_GOTLine1.getLocalBounds().left + _GOTLine1.getLocalBounds().width / 2.0f,
                            _GOTLine1.getLocalBounds().top + _GOTLine1.getLocalBounds().height / 2.0f);
        _GOTLine1.setPosition(SCREENWIDTH / 2.0f, 50);

        _GOTLine2.setFont(_Font);
        _GOTLine2.setColor(sf::Color(200, 0, 0));
        _GOTLine2.setCharacterSize(40);
        _GOTLine2.setString("Your score was: ");
        _GOTLine2.setPosition(SCREENWIDTH / 2 - 225,
                              _GOTLine1.getGlobalBounds().top + _GOTLine1.getLocalBounds().height * 1.2f);

        _GOTLine3.setFont(_Font);
        _GOTLine3.setColor(sf::Color(200, 0, 0));
        _GOTLine3.setCharacterSize(40);
        _GOTLine3.setString("Enter your name:");
        _GOTLine3.setPosition(_GOTLine2.getPosition().x,
                              _GOTLine2.getGlobalBounds().top + _GOTLine2.getLocalBounds().height + 10);
    }

    sf::Vector2f ButtonSize = sf::Vector2f(150, 50);

    std::shared_ptr<Textbox> box(new Textbox(
            sf::Vector2f(_GOTLine3.getPosition().x + _GOTLine3.getLocalBounds().width + 20,
                         _GOTLine3.getPosition().y + 10),
            sf::Vector2f(450 - _GOTLine3.getLocalBounds().width - 20, _GOTLine3.getLocalBounds().height), 25, "Name",
            true));
    _MenuItems.push_back(box);
    std::shared_ptr<MenuButton> button1(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2 + 200, 735), ButtonSize, MenuResult::SubmitScore, "Submit",
                           TextAlignment::Center));
    _MenuItems.push_back(button1);
    std::shared_ptr<MenuButton> button2(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 200, 735), ButtonSize, MenuResult::Back, "Back",
                           TextAlignment::Center));
    _MenuItems.push_back(button2);

//	_JoystickSelection = 1;
}

void GameOverMenu::render(sf::RenderWindow &window) {
    _GOTLine2.setString("Your score was: " + std::to_string(_Highscore.getScore()));

    window.draw(_GOTLine1);
    window.draw(_GOTLine2);
    window.draw(_GOTLine3);

    _Highscore.render(window);

    Menu::render(window);
}

void GameOverMenu::update(int score, int level) {
    _Highscore.setScore(score);
    _Level = level;
}

void GameOverMenu::playSounds() {
    if (_GameOverSound.getStatus() == sf::Sound::Stopped || _GameOverSound.getStatus() == sf::Sound::Paused) {
        if (!_SoundPlayed) {
            _GameOverSound.play();
            _SoundPlayed = true;
        }
    }
}

void GameOverMenu::setVolume(float volume) {
    if (volume > 0) {
        _GameOverSound.setVolume(volume + 10);
    } else {
        _GameOverSound.setVolume(volume);
    }
}

void GameOverMenu::handleEvents(sf::Event &event) {

//		float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
//		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

//		handleJoystick(X);

        // FIXME handle gamestate change
    _FW.setGameState(handleMenuItems(event));
}

GameState GameOverMenu::handleMenuItemResult(MenuResult result) {
    std::string name = _MenuItems[0]->getText();
    switch (result) {
        case MenuResult::Back:
            _SoundPlayed = false;
            _ScoreSubmitted = false;
            _MenuItems[0]->setEnabled(true);
            _MenuItems[1]->setEnabled(true);
            return GameState::Main;
            break;
        case MenuResult::SubmitScore:
            if (!_ScoreSubmitted && name != "") {
                _Highscore.placePlayer(name, _Level);
                _ScoreSubmitted = true;
                _MenuItems[0]->setEnabled(false);
                _MenuItems[1]->setEnabled(false);
//			_JoystickSelection = 2;
            }
            break;
    }
    return _MenuGameState;
}

void GameOverMenu::load() {
    if (_GameOverSoundBuffer.loadFromFile("Resources/Sound/gameOver.wav")) {
        _GameOverSound.setBuffer(_GameOverSoundBuffer);
    }
}
