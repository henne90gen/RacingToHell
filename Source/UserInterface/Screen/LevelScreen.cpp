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
    if (_FW.getCurrentGameState() == GameState::Running) {
        if (event.type == sf::Event::EventType::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            _FW.getLevelManager().setMoving(false);
            _FW.getSoundManager().getLevelMusic()->pause();
            _FW.getSoundManager().getMenuMusic()->play();
            _FW.advanceToGameState(GameState::Pause);
            return;
        }
    }
}

void LevelScreen::update(float frameTime) {

}
