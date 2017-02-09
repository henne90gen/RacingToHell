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


    sf::Vector2f ButtonSize = sf::Vector2f(150, 50);

    std::shared_ptr<Textbox> box = std::make_shared<Textbox>(
            sf::Vector2f(_GOTLine3.getPosition().x + _GOTLine3.getLocalBounds().width + 20,
                         _GOTLine3.getPosition().y + 10),
            sf::Vector2f(450 - _GOTLine3.getLocalBounds().width - 20, _GOTLine3.getLocalBounds().height), 25, "Name",
            MenuResult::SubmitScore, font, true);
    _MenuItems.push_back(box);

    std::shared_ptr<MenuButton> submitBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2 + 200, 735),
                                                                         ButtonSize, MenuResult::SubmitScore, font,
                                                                         "Submit", TextAlignment::Center);
    _MenuItems.push_back(submitBtn);

    std::shared_ptr<MenuButton> backBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2 - 200, 735),
                                                                       ButtonSize, MenuResult::Back, font, "Back",
                                                                       TextAlignment::Center);
    _MenuItems.push_back(backBtn);

    _FW.getSoundManager().playGameOverSound();
}

void GameOverMenu::render(sf::RenderWindow &window) {
    // TODO add background for text
    window.draw(_GOTLine1);
    window.draw(_GOTLine2);
    window.draw(_GOTLine3);

    _HighscoreList.render(window);

    Menu::render(window);
}

void GameOverMenu::handleEvent(sf::Event &event) {
    std::string name = _MenuItems[0]->getText();
    switch (getMenuItemResult(event)) {
        case MenuResult::Back:
            _ScoreSubmitted = false;
            _MenuItems[0]->setEnabled(true);
            _MenuItems[1]->setEnabled(true);
            _FW.reset();
            _FW.advanceToGamState(GameState::MainMenu);
            break;
        case MenuResult::SubmitScore:
            if (!_ScoreSubmitted && name != "") {
                _FW.getHighscoreManager().saveScoreWithName(name);
                _ScoreSubmitted = true;
                _MenuItems[0]->setEnabled(false);
                _MenuItems[1]->setEnabled(false);
            }
            break;
        default:
            break;
    }
}

void GameOverMenu::update(float frameTime) {
    _GOTLine2.setString("Your score was: " + std::to_string((int) _FW.getLevelManager().getScore()));
    _HighscoreList.update(frameTime);
}
