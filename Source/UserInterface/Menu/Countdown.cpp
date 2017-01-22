#include "stdafx.h"
#include "UserInterface/Menu/Countdown.h"
#include "Framework/Framework.h"

Countdown::Countdown(Framework &framework) : Menu(framework, GameState::Countdown),
                                             _AdditionalTime(0.0f) {
    _Text.setString("4");
    _Text.setCharacterSize(75);
    _Text.setOrigin(sf::Vector2f(_Text.getLocalBounds().width / 2.0f, _Text.getLocalBounds().height / 2.0f));
    _Text.setPosition(SCREENWIDTH / 2, SCREENHEIGHT / 2);

    _Background.setSize(sf::Vector2f(SCREENWIDTH, SCREENHEIGHT));
    _Background.setPosition(sf::Vector2f(0, 0));
    _Background.setFillColor(sf::Color(0, 0, 0));
}

Countdown::~Countdown() {
}

void Countdown::render(sf::RenderWindow &window) {
    window.draw(_Background);
    window.draw(_Text);
}

void Countdown::handleEvent(sf::Event &event) {
    _FW.setGameState(handleMenuItems(event));
}

GameState Countdown::handleMenuItemResult(MenuResult result) {
    return _MenuGameState;
}

void Countdown::update(float frameTime) {
    if (!_Started) {
        _Timer.restart();
        _Started = true;
    }

    _Text.setString(std::to_string((int) (4 - (_Timer.getElapsedTime().asSeconds() + _AdditionalTime))));

    // FIXME set some variable instead
//    return (_Timer.getElapsedTime().asSeconds() + _AdditionalTime > 3);
}

void Countdown::fastForward(float time) {
    _AdditionalTime = time;
    std::cout << "Adding " << time << " seconds." << std::endl;
}
