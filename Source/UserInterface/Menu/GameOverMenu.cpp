#include "stdafx.h"
#include "UserInterface/Menu/GameOverMenu.h"
#include "Framework/Framework.h"

GameOverMenu::GameOverMenu(Framework &framework) : Menu(framework, GameState::GameOver),
                                                   _ScoreSubmitted(false),
                                                   _HighscoreList(_FW, sf::Vector2f(SCREENWIDTH / 2 - 225, 190)) {
    sf::Font &font = _FW.getOptionsManager().getFont();

    _GOTLine1.setFont(font);
    _GOTLine1.setFillColor(sf::Color(200, 0, 0));
    _GOTLine1.setCharacterSize(60);
    _GOTLine1.setString("Game Over!");
    _GOTLine1.setOrigin(_GOTLine1.getLocalBounds().left + _GOTLine1.getLocalBounds().width / 2.0f,
                        _GOTLine1.getLocalBounds().top + _GOTLine1.getLocalBounds().height / 2.0f);
    _GOTLine1.setPosition(SCREENWIDTH / 2.0f, 50);

    _GOTLine2.setFont(font);
    _GOTLine2.setFillColor(sf::Color(200, 0, 0));
    _GOTLine2.setCharacterSize(40);
    _GOTLine2.setString("Your score was: ");
    _GOTLine2.setPosition(SCREENWIDTH / 2 - 225,
                          _GOTLine1.getGlobalBounds().top + _GOTLine1.getLocalBounds().height * 1.2f);

    _GOTLine3.setFont(font);
    _GOTLine3.setFillColor(sf::Color(200, 0, 0));
    _GOTLine3.setCharacterSize(40);
    _GOTLine3.setString("Enter your name:");
    _GOTLine3.setPosition(_GOTLine2.getPosition().x,
                          _GOTLine2.getGlobalBounds().top + _GOTLine2.getLocalBounds().height + 10);


    sf::Vector2f buttonSize = sf::Vector2f(150, 50);

    sf::Vector2f textBoxPosition = sf::Vector2f(_GOTLine3.getPosition().x + _GOTLine3.getLocalBounds().width + 20,
                                                _GOTLine3.getPosition().y + 10);
    sf::Vector2f textBoxSize = sf::Vector2f(450 - _GOTLine3.getLocalBounds().width - 20,
                                            _GOTLine3.getLocalBounds().height);
    _TextBox = Textbox(textBoxPosition, textBoxSize, 25, "Name", font, true, false);

    _SubmitBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2 + 200, 735), buttonSize, font, "Submit",
                            TextAlignment::Center);

    _BackBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 200, 735), buttonSize, font, "Back", TextAlignment::Center);

    _FW.getSoundManager().playGameOverSound();
}

void GameOverMenu::render(sf::RenderWindow &renderWindow) {
    // TODO add background for text
    renderWindow.draw(_GOTLine1);
    renderWindow.draw(_GOTLine2);
    renderWindow.draw(_GOTLine3);

    _TextBox.render(renderWindow);
    _SubmitBtn.render(renderWindow);
    _BackBtn.render(renderWindow);

    _HighscoreList.render(renderWindow);
}

void GameOverMenu::handleEvent(sf::Event &event) {

    menuItemTriggered(event, _TextBox);

    std::string name = _TextBox.getValue();

    if (menuItemTriggered(event, _SubmitBtn)) {
        if (!_ScoreSubmitted && name != "") {
            _FW.getHighscoreManager().saveScoreWithName(name);
            _ScoreSubmitted = true;
            _TextBox.setEnabled(false);
            _SubmitBtn.setEnabled(false);
        }
    } else if (menuItemTriggered(event, _BackBtn)) {
        _ScoreSubmitted = false;
        _FW.reset();
        _FW.advanceToGameState(GameState::MainMenu);
    }
}

void GameOverMenu::update(float frameTime) {
    _GOTLine2.setString("Your score was: " + std::to_string((int) _FW.getLevelManager().getScore()));
    _HighscoreList.update(frameTime);
}
