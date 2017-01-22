#include "stdafx.h"
#include "UserInterface/Menu/AboutMenu.h"
#include "Framework/Framework.h"

AboutMenu::AboutMenu(Framework &framework) : Menu(framework, GameState::About) {
    sf::Vector2f ButtonSize = sf::Vector2f(150, 50);
    std::shared_ptr<MenuButton> button(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 200, 735), ButtonSize, MenuResult::Back, "Back",
                           TextAlignment::Center));
    _MenuItems.push_back(button);

    _Frame.setPosition(sf::Vector2f(50, 75));
    _Frame.setSize(sf::Vector2f(SCREENWIDTH - 100, 645));
    _Frame.setFillColor(sf::Color(0, 0, 0, 175));
    _Frame.setOutlineThickness(1);
    _Frame.setOutlineColor(sf::Color::Black);

    sf::Font &font = _FW.getOptionsManager().getFont();

    _Text.setFont(font);
    _Text.setStyle(sf::Text::Bold);
    _Text.setColor(sf::Color::White);
    _Text.setString("This game was made by");
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 110));

    _Text2.setFont(font);
    _Text2.setStyle(sf::Text::Bold);
    _Text2.setColor(sf::Color::White);
    _Text2.setString("Alex Ivliev and Hendrik Müller");
    _Text2.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text2.getLocalBounds().width / 2, 145));

    _Music.setFont(font);
    _Music.setCharacterSize(25);
    _Music.setColor(sf::Color::White);
    _Music.setString("Music by KsTBeats");
    _Music.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Music.getLocalBounds().width / 2, 250));

    _Cars.setFont(font);
    _Cars.setCharacterSize(25);
    _Cars.setColor(sf::Color::White);
    _Cars.setString("Enemy cars by sujit1717");
    _Cars.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Cars.getLocalBounds().width / 2, 300));

    _Boss.setFont(font);
    _Boss.setCharacterSize(25);
    _Boss.setColor(sf::Color::White);
    _Boss.setString("Boss sprites by Skorpio,");
    _Boss.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Boss.getLocalBounds().width / 2, 350));

    _Boss2.setFont(font);
    _Boss2.setCharacterSize(25);
    _Boss2.setColor(sf::Color::White);
    _Boss2.setString("MillionthVector and Bleed");
    _Boss2.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Boss2.getLocalBounds().width / 2, 385));

    _LevelUp.setFont(font);
    _LevelUp.setCharacterSize(25);
    _LevelUp.setColor(sf::Color::White);
    _LevelUp.setString("'LevelUp sound effect'");
    _LevelUp.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _LevelUp.getLocalBounds().width / 2, 435));

    _LevelUp2.setFont(font);
    _LevelUp2.setCharacterSize(25);
    _LevelUp2.setColor(sf::Color::White);
    _LevelUp2.setString("by Bart Kelsey");
    _LevelUp2.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _LevelUp2.getLocalBounds().width / 2, 470));

    _AIShotSounds = _Music;
    _AIShotSounds.setString("Sound of enemy cars");
    _AIShotSounds.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _AIShotSounds.getLocalBounds().width / 2, 520));

    _AIShotSounds2 = _Music;
    _AIShotSounds2.setString("www.youtube.com/blinkfarm");
    _AIShotSounds2.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _AIShotSounds2.getLocalBounds().width / 2, 555));

    _PlayerCarShotSounds = _Music;
    _PlayerCarShotSounds.setString("Sound of player car");
    _PlayerCarShotSounds.setPosition(
            sf::Vector2f(SCREENWIDTH / 2 - _PlayerCarShotSounds.getLocalBounds().width / 2, 605));

    _PlayerCarShotSounds2 = _Music;
    _PlayerCarShotSounds2.setString("soundbible.com/1804-M4A1-Single.html");
    _PlayerCarShotSounds2.setPosition(
            sf::Vector2f(SCREENWIDTH / 2 - _PlayerCarShotSounds2.getLocalBounds().width / 2, 640));
}

void AboutMenu::render(sf::RenderWindow &window) {
    window.draw(_Frame);
    window.draw(_Text);
    window.draw(_Text2);
    window.draw(_Music);
    window.draw(_Cars);
    window.draw(_Boss);
    window.draw(_Boss2);
    window.draw(_LevelUp);
    window.draw(_LevelUp2);
    window.draw(_AIShotSounds);
    window.draw(_AIShotSounds2);
    window.draw(_PlayerCarShotSounds);
    window.draw(_PlayerCarShotSounds2);

    Menu::render(window);
}

void AboutMenu::handleEvent(sf::Event &event) {
    _FW.setGameState(handleMenuItems(event));
}

GameState AboutMenu::handleMenuItemResult(MenuResult result) {
    switch (result) {
        case MenuResult::Back:
            return _FW.getLastGameState();
    }
    return _MenuGameState;
}
