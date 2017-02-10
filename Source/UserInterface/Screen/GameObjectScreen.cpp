//
// Created by henne on 02.12.16.
//

#include "UserInterface/Screen/GameObjectScreen.h"
#include "Framework/Framework.h"

GameObjectScreen::GameObjectScreen(Framework &framework) : GameScreen(framework) {

}

void GameObjectScreen::render(sf::RenderWindow &renderWindow) {
    for (unsigned int i = 0; i < _FW.getGOM().getCars().size(); i++) {
        _FW.getGOM().getCars().at(i)->render(renderWindow);
    }

    for (unsigned int i = 0; i < _FW.getGOM().getPickupItems().size(); i++) {
        _FW.getGOM().getPickupItems().at(i)->render(renderWindow);
    }

    for (unsigned int i = 0; i < _FW.getGOM().getBullets().size(); i++) {
        _FW.getGOM().getBullets().at(i)->render(renderWindow);
    }

    // TODO render boss fight
//    if (_BossFight) {
//        _Boss->render(renderWindow);
//    }

    _FW.getGOM().getPlayerCar()->render(renderWindow, !_FW.isMouseVisible());

    for (unsigned int i = 0; i < _FW.getGOM().getAnimations().size(); i++) {
        _FW.getGOM().getAnimations().at(i)->render(renderWindow);
    }
}

void GameObjectScreen::update(float frameTime) {
    std::vector<std::shared_ptr<Animation>> &animations = _FW.getGOM().getAnimations();
    for (int i = 0; i < animations.size(); i++) {
        if (animations[i]->getAnimationState() == Animation::AnimationState::Stop) {
            animations.erase(animations.begin() + i);
            i--;
        } else {
            animations[i]->update(frameTime);
        }
    }
}

void GameObjectScreen::handleEvent(sf::Event &event) {
    if (_FW.getCurrentGameState() == GameState::Running) {
        if (event.type == sf::Event::EventType::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            _FW.getLevelManager().setMoving(false);
            _FW.advanceToGameState(GameState::Pause);
            return;
        }
        if (_FW.getGOM().getPlayerCar()->isAlive()) {
            _FW.getGOM().getPlayerCar()->handleEvent(event);
            // TODO maybe move this into player
            _FW.getGOM().getPlayerCar()->applyKeyPress(_FW.getGOM().getPlayerCar()->getPressedKeys());
        }
    }
}
