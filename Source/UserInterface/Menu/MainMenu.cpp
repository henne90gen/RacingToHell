#include "stdafx.h"
#include "UserInterface/Menu/MainMenu.h"
#include "Framework/Framework.h"

MainMenu::MainMenu(Framework &framework) : Menu(framework, GameState::MainMenu) {

    sf::Font &font = _FW.getOptionsManager().getFont();

    //Menu-Items
    sf::Vector2f ButtonSize = sf::Vector2f(250, 50);

    _PlayBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 100), ButtonSize, font, "Play", TextAlignment::Center);

    _MultiplayerBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 170), ButtonSize, font, "Multiplayer",
                                 TextAlignment::Center);
    _MultiplayerBtn.setEnabled(false);

    _HighscoreBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 240), ButtonSize, font, "Highscores",
                               TextAlignment::Center);

    _OptionsBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 310), ButtonSize, font, "Options", TextAlignment::Center);

    _ExitBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 380), ButtonSize, font, "Exit", TextAlignment::Center);

    _LeftBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 60, SCREENHEIGHT - 335), sf::Vector2f(0, 50), font, "<<",
                          TextAlignment::Left);

    _RightBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2 + 50, SCREENHEIGHT - 335), sf::Vector2f(0, 50), font, ">>",
                           TextAlignment::Left);

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
