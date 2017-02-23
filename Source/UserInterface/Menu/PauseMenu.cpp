#include "stdafx.h"
#include "UserInterface/Menu/PauseMenu.h"
#include "Framework/Framework.h"

PauseMenu::PauseMenu(Framework &framework) : Menu(framework, GameState::Pause) {

    sf::Font &font = _FW.getOptionsManager().getFont();

    //Menu-Items
    sf::Vector2f ButtonSize = sf::Vector2f(250, 50);

    _ResumeBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 240), ButtonSize, font, "Resume", TextAlignment::Center);

    _OptionsBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 310), ButtonSize, font, "Options", TextAlignment::Center);

    _BackToMainBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 380), ButtonSize, font, "Main Menu",
                                TextAlignment::Center);

    _ExitBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 450), ButtonSize, font, "Exit", TextAlignment::Center);

    //Menu-Text
    _Text.setString("Pause Menu");
    _Text.setCharacterSize(53);
    _Text.setStyle(sf::Text::Style::Bold);
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));
}

void PauseMenu::render(sf::RenderWindow &renderWindow) {
    renderWindow.draw(_Text);

    _ResumeBtn.render(renderWindow);
    _OptionsBtn.render(renderWindow);
    _BackToMainBtn.render(renderWindow);
    _ExitBtn.render(renderWindow);
}

void PauseMenu::handleEvent(sf::Event &event) {
    if (menuItemTriggered(event, _ResumeBtn)) {
        _FW.getLevelManager().setMoving(true);
        _FW.getSoundManager().getLevelMusic()->play();
        _FW.getSoundManager().getMenuMusic()->pause();
        _FW.goBackGameState();
    } else if (menuItemTriggered(event, _OptionsBtn)) {
        _FW.advanceToGameState(GameState::Options);
    } else if (menuItemTriggered(event, _BackToMainBtn)) {
        _FW.reset();
        _FW.getSoundManager().getLevelMusic()->stop();
        _FW.advanceToGameState(GameState::MainMenu);
    } else if (menuItemTriggered(event, _ExitBtn)) {
        _FW.getSoundManager().getLevelMusic()->stop();
        _FW.getSoundManager().getMenuMusic()->stop();
        _FW.advanceToGameState(GameState::Exiting);
    }
}
