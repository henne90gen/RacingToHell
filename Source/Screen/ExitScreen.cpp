//
// Created by henne on 30.11.16.
//

#include "stdafx.h"
#include "Screen/ExitScreen.h"
#include "Framework/Framework.h"

ExitScreen::ExitScreen(Framework &framework) : GameScreen(framework), _TotalFadeTime(2.0f), _FadeSpeed(15.0f), _InitialScale(10.0f) {

    _Font.loadFromFile("Resources/Font/arial.ttf");
    _GoodByeText.setFont(_Font);
    _GoodByeText.setString("Good bye, see you soon.");
    _GoodByeText.setCharacterSize(40);
    _GoodByeText.setColor(sf::Color::White);
    _GoodByeText.setStyle(sf::Text::Style::Bold);
    _GoodByeText.setOrigin(
            sf::Vector2f(_GoodByeText.getLocalBounds().width / 2, _GoodByeText.getLocalBounds().height / 2));
    _GoodByeText.setPosition(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT / 2));

    _Background.setFillColor(sf::Color::Black);
    _Background.setSize(sf::Vector2f(SCREENWIDTH, SCREENHEIGHT));
}

void ExitScreen::render(sf::RenderWindow &renderWindow) {
    renderWindow.draw(_Background);
    renderWindow.draw(_GoodByeText);
}

void ExitScreen::handleEvent(sf::Event &event) {

}

void ExitScreen::update(float frameTime) {
    float seconds = _FadeTimer.getElapsedTime().asSeconds();
    if (seconds >= _TotalFadeTime) {
        _FW.stop();
    }

    float scale = _InitialScale - _FadeSpeed * seconds / _TotalFadeTime;
    if (scale < 1) {
        scale = 1;
    }
    _GoodByeText.setScale(sf::Vector2f(scale, scale));
}
