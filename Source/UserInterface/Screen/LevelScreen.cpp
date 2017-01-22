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

    int newLevel = _FW.getLevelManager().getLevel();

//    if (_Sprite.getPosition().y + frameTime * newLevel >= 0) {
//        _Sprite.setPosition(sf::Vector2f(0, -1600));
//    } else {
//        _Sprite.setPosition(
//                sf::Vector2f(_Sprite.getPosition().x,
//                             _Sprite.getPosition().y + frameTime * _FW.getLevelManager().getRoadSpeed()));
//    }

//    if (newLevel != _CurrentLevel) {
//        _CurrentLevel = newLevel;
//        _Sprite.setTexture((*_Textures.at((newLevel - 1) % _Textures.size())));
//        _Music.openFromFile(
//                "Resources/Sound/Music/newLevel" + std::to_string((newLevel - 1) % _Textures.size() + 1) + ".ogg");
//    }

//    if (_FW.getLevelManager().isResettingLevel()) {
//        _Sprite.setTexture((*_Textures.at(0)));
//	_Music.openFromFile("Resources/Sound/Music/level" + std::to_string((_Level - 1) % _Textures.size() + 1) + ".ogg");
//        _FW.getLevelManager().stopResettingLevel();
//    }
}
