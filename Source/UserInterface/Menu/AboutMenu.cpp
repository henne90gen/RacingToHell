#include "stdafx.h"
#include "UserInterface/Menu/AboutMenu.h"
#include "Framework/Framework.h"

AboutMenu::AboutMenu(Framework &framework) : Menu(framework, GameState::About) {

    sf::Font &font = _FW.getOptionsManager().getFont();

    sf::Vector2f ButtonSize = sf::Vector2f(150, 50);
    _BackBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 200, 735), ButtonSize, font, "Back", TextAlignment::Center);

    _Frame.setPosition(sf::Vector2f(50, 75));
    _Frame.setSize(sf::Vector2f(SCREENWIDTH - 100, 645));
    _Frame.setFillColor(sf::Color(0, 0, 0, 175));
    _Frame.setOutlineThickness(1);
    _Frame.setOutlineColor(sf::Color::Black);

    _Text.setFont(font);
    _Text.setStyle(sf::Text::Bold);
    _Text.setString("This game was made by");
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 110));

    _Text2.setFont(font);
    _Text2.setStyle(sf::Text::Bold);
    _Text2.setString("Alex Ivliev and Hendrik Müller");
    _Text2.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text2.getLocalBounds().width / 2, 145));

    _Music.setFont(font);
    _Music.setCharacterSize(25);
    _Music.setString("Music by KsTBeats");
    _Music.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Music.getLocalBounds().width / 2, 250));

    _Cars.setFont(font);
    _Cars.setCharacterSize(25);
    _Cars.setString("Enemy cars by sujit1717");
    _Cars.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Cars.getLocalBounds().width / 2, 300));

    _Boss.setFont(font);
    _Boss.setCharacterSize(25);
    _Boss.setString("Boss sprites by Skorpio,");
    _Boss.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Boss.getLocalBounds().width / 2, 350));

    _Boss2.setFont(font);
    _Boss2.setCharacterSize(25);
    _Boss2.setString("MillionthVector and Bleed");
    _Boss2.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Boss2.getLocalBounds().width / 2, 385));

    _LevelUp.setFont(font);
    _LevelUp.setCharacterSize(25);
    _LevelUp.setString("'LevelUp sound effect'");
    _LevelUp.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _LevelUp.getLocalBounds().width / 2, 435));

    _LevelUp2.setFont(font);
    _LevelUp2.setCharacterSize(25);
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

void AboutMenu::render(sf::RenderWindow &renderWindow) {
    renderWindow.draw(_Frame);
    renderWindow.draw(_Text);
    renderWindow.draw(_Text2);
    renderWindow.draw(_Music);
    renderWindow.draw(_Cars);
    renderWindow.draw(_Boss);
    renderWindow.draw(_Boss2);
    renderWindow.draw(_LevelUp);
    renderWindow.draw(_LevelUp2);
    renderWindow.draw(_AIShotSounds);
    renderWindow.draw(_AIShotSounds2);
    renderWindow.draw(_PlayerCarShotSounds);
    renderWindow.draw(_PlayerCarShotSounds2);

    _BackBtn.render(renderWindow);
}

void AboutMenu::handleEvent(sf::Event &event) {
    if (menuItemTriggered(event, _BackBtn)) {
        _FW.goBackGameState();
    }
}
