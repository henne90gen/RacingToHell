#include "stdafx.h"
#include "UserInterface/Menu/HighscoreMenu.h"
#include "Framework/Framework.h"

HighscoreMenu::HighscoreMenu(Framework &framework) : Menu(framework, GameState::Highscores),
                                                     _HighscoreList(_FW, sf::Vector2f(75, 100)) {

    sf::Font &font = _FW.getOptionsManager().getFont();

    sf::Vector2f ButtonSize = sf::Vector2f(150, 50);
    _BackBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 200, 735), ButtonSize, font, "Back", TextAlignment::Center);

    _Text.setString("Highscores");
    _Text.setCharacterSize(53);
    _Text.setFillColor(sf::Color::White);
    _Text.setStyle(sf::Text::Style::Bold);
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));
}

void HighscoreMenu::render(sf::RenderWindow &renderWindow) {
    _HighscoreList.render(renderWindow);
    _BackBtn.render(renderWindow);
}

void HighscoreMenu::update(float frameTime) {
    _FW.getHighscoreManager().setHighlightedRow(-1);
    _FW.getHighscoreManager().setShowAdditionalLine(false);
    _HighscoreList.update(frameTime);
}

void HighscoreMenu::handleEvent(sf::Event &event) {
    if (menuItemTriggered(event, _BackBtn)) {
        _FW.goBackGameState();
    }
}
