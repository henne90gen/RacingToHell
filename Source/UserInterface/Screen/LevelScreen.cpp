//
// Created by henne on 29.11.16.
//

#include "stdafx.h"
#include "UserInterface/Screen/LevelScreen.h"
#include "Framework/Framework.h"

LevelScreen::LevelScreen(Framework &framework) : GameScreen(framework) {}

void LevelScreen::render(sf::RenderWindow &renderWindow) {
    renderWindow.draw(_FW.getLevelManager().getSprite());
}

void LevelScreen::handleEvent(sf::Event &event) {

}

void LevelScreen::update(float frameTime) {

}
