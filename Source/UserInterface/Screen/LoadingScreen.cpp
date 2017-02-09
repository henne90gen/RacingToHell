#include "stdafx.h"
#include "UserInterface/Screen/LoadingScreen.h"
#include "Framework/Framework.h"

LoadingScreen::LoadingScreen(Framework &framework) : GameScreen(framework), _Alpha(255), _FadeTime(0.5f) {
    _Text.setString("Loading...");
    _Text.setFont(_FW.getOptionsManager().getFont());
    _Text.setCharacterSize(75);
    _Text.setOrigin(sf::Vector2f(_Text.getLocalBounds().width / 2.0f, _Text.getLocalBounds().height / 2.0f));
    _Text.setPosition(SCREENWIDTH / 2, SCREENHEIGHT / 2);

    _Background.setSize(sf::Vector2f(SCREENWIDTH, SCREENHEIGHT));
    _Background.setPosition(sf::Vector2f(0, 0));
    _Background.setFillColor(sf::Color(0, 0, 0));
}

void LoadingScreen::render(sf::RenderWindow &window) {
    sf::Uint8 br = _Background.getFillColor().r;
    sf::Uint8 bg = _Background.getFillColor().g;
    sf::Uint8 bb = _Background.getFillColor().b;
    const sf::Color &backgroundColor = sf::Color(br, bg, bb, _Alpha);
    _Background.setFillColor(backgroundColor);
    window.draw(_Background);

    sf::Uint8 tr = _Text.getFillColor().r;
    sf::Uint8 tg = _Text.getFillColor().g;
    sf::Uint8 tb = _Text.getFillColor().b;
    const sf::Color &textColor = sf::Color(tr, tg, tb, _Alpha);
    _Text.setFillColor(textColor);
    window.draw(_Text);
}

void LoadingScreen::handleEvent(sf::Event &event) {}

void LoadingScreen::update(float frameTime) {
    if (_FW.getCurrentGameState() == GameState::LoadingToMain) {
        _Alpha -= 255.0f / _FadeTime * frameTime;
        if (_Alpha <= 10) {
            _FW.advanceToGameState(GameState::MainMenu);
        }
    }
}
