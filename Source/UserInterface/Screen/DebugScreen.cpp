//
// Created by henne on 22.01.17.
//

#include "stdafx.h"
#include "UserInterface/Screen/DebugScreen.h"
#include "Framework/Framework.h"

DebugScreen::DebugScreen(Framework &framework) : GameScreen(framework) {
    sf::Font &font = _FW.getOptionsManager().getFont();

    unsigned int characterSize = 14;
    int topSpacing = 50;
    int leftSpacing = 0;
    int verticalSpacing = 15;
    int horizontalSpacing = 70;

    _FPSLabel = sf::Text("FPS:", font, characterSize);
    _FPSLabel.setPosition(leftSpacing, topSpacing);

    _UPSLabel = sf::Text("UPS:", font, characterSize);
    _UPSLabel.setPosition(leftSpacing, topSpacing + verticalSpacing);

    _BulletsOnScreenLabel = sf::Text("Bullets:", font, characterSize);
    _BulletsOnScreenLabel.setPosition(leftSpacing, topSpacing + 2 * verticalSpacing);

    _CarsOnScreenLabel = sf::Text("Cars:", font, characterSize);
    _CarsOnScreenLabel.setPosition(leftSpacing, topSpacing + 3 * verticalSpacing);

    _AICarHealthLabel = sf::Text("AI health:", font, characterSize);
    _AICarHealthLabel.setPosition(leftSpacing, topSpacing + 4 * verticalSpacing);


    _FPS = sf::Text("", font, characterSize);
    _FPS.setPosition(horizontalSpacing, topSpacing);

    _UPS = sf::Text("", font, characterSize);
    _UPS.setPosition(leftSpacing + horizontalSpacing, topSpacing + 1 * verticalSpacing);

    _BulletsOnScreen = sf::Text("", font, characterSize);
    _BulletsOnScreen.setPosition(leftSpacing + horizontalSpacing, topSpacing + 2 * verticalSpacing);

    _CarsOnScreen = sf::Text("", font, characterSize);
    _CarsOnScreen.setPosition(leftSpacing + horizontalSpacing, topSpacing + 3 * verticalSpacing);

    _AICarHealth = sf::Text("", font, characterSize);
    _AICarHealth.setPosition(leftSpacing + horizontalSpacing, topSpacing + 4 * verticalSpacing);
}

void DebugScreen::render(sf::RenderWindow &renderWindow) {
    renderWindow.draw(_FPSLabel);
    renderWindow.draw(_UPSLabel);
    renderWindow.draw(_BulletsOnScreenLabel);
    renderWindow.draw(_CarsOnScreenLabel);
    renderWindow.draw(_AICarHealthLabel);

    renderWindow.draw(_FPS);
    renderWindow.draw(_UPS);
    renderWindow.draw(_BulletsOnScreen);
    renderWindow.draw(_CarsOnScreen);
    renderWindow.draw(_AICarHealth);
}

void DebugScreen::handleEvent(sf::Event &event) {

}

void DebugScreen::update(float frameTime) {
    _FPS.setString(std::to_string(_FW.getFPS()));
    _UPS.setString(std::to_string(_FW.getUPS()));

    unsigned long bullets = _FW.getGOM().getBullets().size();
    _BulletsOnScreen.setString(std::to_string(bullets));

    unsigned long cars = _FW.getGOM().getCars().size() + 1;
    _CarsOnScreen.setString(std::to_string(cars));

    _AICarHealth.setString(std::to_string(_FW.getLevelManager().getAIHP()));
}
