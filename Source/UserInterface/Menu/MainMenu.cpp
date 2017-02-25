#include "stdafx.h"
#include "UserInterface/Menu/MainMenu.h"
#include "Framework/Framework.h"

MainMenu::MainMenu(Framework &framework) : Menu(framework, GameState::MainMenu) {

    sf::Font &font = _FW.getOptionsManager().getFont();

    //Menu-Items
    sf::Vector2f buttonSize = sf::Vector2f(250, 50);

    _PlayBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 100), buttonSize, font, "Play", TextAlignment::Center);

    _MultiplayerBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 170), buttonSize, font, "Multiplayer",
                                 TextAlignment::Center);
    _MultiplayerBtn.setEnabled(false);

    _HighscoreBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 240), buttonSize, font, "Highscores",
                               TextAlignment::Center);

    _OptionsBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 310), buttonSize, font, "Options", TextAlignment::Center);

    _ExitBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 380), buttonSize, font, "Exit", TextAlignment::Center);

    sf::Vector2f arrowSize = sf::Vector2f(80, 40);
    _LeftBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 90, 480), arrowSize, font, "<<",
                          TextAlignment::Center);

    _RightBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2 + 90, 480), arrowSize, font, ">>",
                           TextAlignment::Center);

    //MainMenu-Menu Text
    _Text.setString("Main Menu");
    _Text.setCharacterSize(53);
    _Text.setStyle(sf::Text::Style::Bold);
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 20));

    //Stats-Box
    _StatBox = std::make_shared<StatBox>(sf::Vector2f(SCREENWIDTH / 2, 575));
}

void MainMenu::render(sf::RenderWindow &renderWindow) {
    renderWindow.draw(_Text);

    _PlayBtn.render(renderWindow);
    _MultiplayerBtn.render(renderWindow);
    _HighscoreBtn.render(renderWindow);
    _OptionsBtn.render(renderWindow);
    _ExitBtn.render(renderWindow);
    _LeftBtn.render(renderWindow);
    _RightBtn.render(renderWindow);

    _StatBox->setCarStats(_FW.getGOM().getPlayerCar()->getPlayerCarIndex());
    _StatBox->render(renderWindow);
}

void MainMenu::handleEvent(sf::Event &event) {
    if (menuItemTriggered(event, _PlayBtn)) {
        _FW.getSoundManager().getLevelMusic()->play();
        _FW.getSoundManager().getMenuMusic()->pause();
        _FW.advanceToGameState(GameState::Running);
    } else if (menuItemTriggered(event, _MultiplayerBtn)) {
        _FW.advanceToGameState(GameState::MultiplayerSelection);
    } else if (menuItemTriggered(event, _HighscoreBtn)) {
        _FW.advanceToGameState(GameState::Highscores);
    } else if (menuItemTriggered(event, _OptionsBtn)) {
        _FW.advanceToGameState(GameState::Options);
    } else if (menuItemTriggered(event, _LeftBtn)) {
        _FW.getGOM().previousPlayerCar();
    } else if (menuItemTriggered(event, _RightBtn)) {
        _FW.getGOM().nextPlayerCar();
    } else if (menuItemTriggered(event, _ExitBtn)) {
        _FW.getSoundManager().getMenuMusic()->stop();
        _FW.advanceToGameState(GameState::Exiting);
    }
}
