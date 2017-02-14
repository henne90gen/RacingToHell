#include "stdafx.h"
#include "Framework/LevelManager.h"
#include "Framework/Framework.h"

void LevelManager::load() {
    std::cout << "Loading level textures..." << std::endl;

    _Textures = new sf::Texture[4];

    for (int i = 1; i <= 4; i++) {
        if (!_Textures[i - 1].loadFromFile("Resources/Texture/Road/Road" + std::to_string(i) + ".jpg")) {
            std::cout << "Couldn't load road texture." << std::endl;
        }
    }

    _Textures[0].setRepeated(true);
    _Sprite.setTexture(_Textures[0]);
    _Sprite.setPosition(sf::Vector2f(0, -1600));
    _Sprite.setTextureRect(sf::IntRect(0, 0, 600, 2400));
}

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
        if (!_FW.getGOM().isInBossFight()) {
            _LevelTime += frameTime;
        }
        if (_FW.getGOM().getPlayerCar()->isAlive()) {
            addScore(ScoreEvent::Tick, frameTime);
        }
        if (_LevelTime >= _TotalLevelTime) {
            if (_FW.getGOM().emptyScreen()) {
                _FW.getGOM().enterBossFight();
                _LevelTime = 0;
            }
        } else if (_FW.getGOM().bossIsDead()) {
            levelUp();
            _FW.advanceToGameState(GameState::LevelUp);
        }
    }
}

void LevelManager::levelUp() {
    _LevelTime = 0;
    _Level++;

    _Textures[(_Level - 1) % 4].setRepeated(true);
    _Sprite.setTexture(_Textures[(_Level - 1) % 4]);

    _FW.getSoundManager().nextLevel();
}

void LevelManager::resetToLevelOne() {
    _Level = 1;
    _TotalLevelTime = 5.0f;
    _LevelTime = 0;
    _ShouldMove = true;
    _Score = 0;

    _Textures[0].setRepeated(true);
    _Sprite.setTexture(_Textures[0]);
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
            // TODO review added points
            _Score += 5000 + 10000 * (int) _FW.getOptionsManager().getDifficulty() *
                             (int) _FW.getOptionsManager().getDifficulty();
            break;
        case ScoreEvent::LevelUp:
            // TODO add points for level up
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
            // FIXME change 200 back to 4500
            return 200 + (int) ((_Level - 1) / 4.0f) * 2500;
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
