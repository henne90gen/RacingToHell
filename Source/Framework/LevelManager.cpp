#include "stdafx.h"
#include "Framework/LevelManager.h"
#include "Framework/Framework.h"

void LevelManager::update(float frameTime) {

    if (!_ShouldMove) {
        return;
    }

    if (_Sprite.getPosition().y + frameTime * getRoadSpeed() >= 0) {
        _Sprite.setPosition(sf::Vector2f(0, -1600));
    } else {
        _Sprite.setPosition(
                sf::Vector2f(_Sprite.getPosition().x, _Sprite.getPosition().y + frameTime * getRoadSpeed()));
    }

    if (_FW.getCurrentGameState() == GameState::Running) {
        _LevelTime += frameTime;
        if (_FW.getGOM().getPlayerCar()->isAlive()) {
            addScore(ScoreEvent::Tick, frameTime);
        }
        if (_LevelTime >= _TotalLevelTime) {
            levelUp();
        }
    }
}

//void LevelManager::playMusic() {
//    if (_Music.getStatus() == sf::Sound::Stopped || _Music.getStatus() == sf::Sound::Paused) {
//        _Music.play();
//    }
//}
//
//void LevelManager::stopMusic() {
//    _Music.stop();
//}
//
//void LevelManager::pauseMusic() {
//    _Music.pause();
//}
//
//void LevelManager::update(float Volume) {
//    _Music.update(Volume);
//}

void LevelManager::levelUp() {
    _LevelTime = 0;
    _Level++;
}

void LevelManager::resetToLevelOne() {
    _Level = 1;
    _TotalLevelTime = 60.0f;
    _LevelTime = 0;
    _IsResettingLevel = true;
    _ShouldMove = true;
    _Score = 0;
}

int LevelManager::getRoadSpeed() {
    // FIXME maybe find some better calculation
    Difficulty difficulty = _FW.getOptionsManager().getDifficulty();
    switch (difficulty) {
        case Difficulty::Easy:
        case Difficulty::Normal:
        case Difficulty::Hard:
            return (60 + ((int) difficulty) * 20) * _Level + 100;
        case Difficulty::Insane:
            return (110 * _Level + 150);
    }
}

void LevelManager::load() {
    std::cout << "Loading level textures..." << std::endl;

    for (int i = 1; i <= 4; i++) {
        std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
        (*texture).loadFromFile("Resources/Texture/Road/Road" + std::to_string(i) + ".jpg");
        _Textures.push_back(texture);
    }
    _Sprite.setTexture((*_Textures.at(0)));
    _Sprite.setPosition(sf::Vector2f(0, -1600));
}

void LevelManager::addScore(ScoreEvent event, float modifier) {
//    float multiplier = _FW.getOptionsManager().getScoreMultiplier();
//    _Score += _GameObjectContainer.getCarScore() * multiplier;
//    switch (_OptionsMenu.getDifficulty()) {
//        case 0:
//            _Score += 5 * _LevelManager.getLevel() * multiplier * _FrameTime;
//            break;
//        case 1:
//            _Score += 10 * (int) std::pow((float) _LevelManager.getLevel(), 1.15f) * multiplier * _FrameTime;
//            break;
//        case 2:
//            _Score += 30 * (int) std::pow((float) _LevelManager.getLevel(), 1.3f) * multiplier * _FrameTime;
//            break;
//        case 3:
//            _Score += 60 * (int) std::pow((float) _LevelManager.getLevel(), 1.6f) * multiplier * _FrameTime;
//            break;
//        default:
//            break;
//    }


    switch (event) {
        case ScoreEvent::Tick: {
            // modifier is frametime and this is being called every frame
            int scorePerSecond = 5;
            // TODO review added points
            _Score += scorePerSecond * modifier;
        }
            break;
        case ScoreEvent::DestroyedCar:
            // TODO review added points
            _Score += 1.5 * modifier;
            break;
        case ScoreEvent::DefeatedBoss:
            // TODO add points for killing a boss
            break;
        case ScoreEvent::LevelUp:
            // TODO add point for a level up
            break;
    }
}

int LevelManager::getAiHP() {
    switch (_FW.getOptionsManager().getDifficulty()) {
        case Difficulty::Easy:
            return 40 + _Level * 10;
        case Difficulty::Normal:
            return 50 + _Level * 15;
        case Difficulty::Hard:
            return 60 + _Level * 20;
        case Difficulty::Insane:
            return 65 + _Level * 25;
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
