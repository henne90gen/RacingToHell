#include "stdafx.h"
#include "Framework/LevelManager.h"
#include "Framework/Framework.h"

void LevelManager::update(float frameTime, GameState gameState) {

    if (!_ShouldMove) {
        return;
    }

    _LevelTime += frameTime;

    if (_Sprite.getPosition().y + frameTime * getRoadSpeed() >= 0) {
        _Sprite.setPosition(sf::Vector2f(0, -1600));
    } else {
        _Sprite.setPosition(sf::Vector2f(_Sprite.getPosition().x, _Sprite.getPosition().y + frameTime * getRoadSpeed()));
    }

    if (gameState == GameState::Running) {
        if (_LevelTime >= _TotalLevelTime) {
            levelUp();
        }
    }
}

void LevelManager::playMusic() {
    if (_Music.getStatus() == sf::Sound::Stopped || _Music.getStatus() == sf::Sound::Paused) {
        _Music.play();
    }
}

void LevelManager::stopMusic() {
    _Music.stop();
}

void LevelManager::pauseMusic() {
    _Music.pause();
}

void LevelManager::setVolume(float Volume) {
    _Music.setVolume(Volume);
}

void LevelManager::levelUp() {
    _LevelTime = 0;
    _Level++;
}

void LevelManager::resetToLevelOne() {
    // TODO set _TotalLevelTime back to 60
    _Level = 1;
    _TotalLevelTime = 60.0f;
    _LevelTime = 0;
    _IsResettingLevel = true;
}

int LevelManager::getRoadSpeed() {
    // FIXME calculation of road speed is not correct
    return 100;
//    switch (_Difficulty) {
//        case 0:
//        case 1:
//        case 2:
//            return (60 + _Difficulty * 20) * _Level + 100;
//        case 3:
//            return (110 * _Level + 150);
//        default:
//            return 100;
//    }
}

void LevelManager::load() {
    for (int i = 1; i <= 4; i++) {
        std::shared_ptr<sf::Texture> texture(new sf::Texture());
        (*texture).loadFromFile("Resources/Texture/Road/Road" + std::to_string(i) + ".jpg");
        _Textures.push_back(texture);
    }
    _Sprite.setTexture((*_Textures.at(0)));
    _Sprite.setPosition(sf::Vector2f(0, -1600));

    _Difficulty = 1;

    // TODO load level music here
}

void LevelManager::addScore(ScoreEvent event, float modifier) {
    switch (event) {
        case ScoreEvent::Tick:break;
        case ScoreEvent::DestroyedCar:
            _Score += (int) (1.5 * modifier);
            break;
        case ScoreEvent::DefeatedBoss:break;
    }
}


int LevelManager::getAIHP() {
    switch (_FW.getOptionsManager().getDifficulty()) {
        case 0:
            return 40 + _Level * 10;
        case 1:
            return 50 + _Level * 15;
        case 2:
            return 60 + _Level * 20;
        case 3:
            return 65 + _Level * 25;
        default:
            return 1;
    }
}

int LevelManager::getBossHP() {
    switch ((_Level - 1) % 4) {
        case 0:
            return 4500 + (int) ((_Level - 1) / 4.0f) * 2500;
        case 1:
            return 5500 + (int) ((_Level - 1) / 4.0f) * 2500;
        case 2:
            return 1500 + 750 * (int) ((_Level - 1) / 4.0f);
        case 3:
            return 6500 + (int) ((_Level - 1) / 4.0f) * 2500;
        default:
            return 1;
    }
}
