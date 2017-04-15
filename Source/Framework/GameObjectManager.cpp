#include "stdafx.h"
#include "Framework/GameObjectManager.h"
#include "Framework/Framework.h"

GameObjectManager::GameObjectManager(Framework &framework) : Manager(framework), _PlayerBulletSpeed(600),
                                                             _AIBulletSpeed(400), _InBossFight(false) {
    _CarFrequency = 1;
    _CanisterFrequency = 0.5;
    _ToolboxFrequency = 0.25;
    _BulletFrequency = 2;

    _Cars.reserve(32);
    _Bullets.reserve(256);
    _Animations.reserve(16);
    _PickupItems.reserve(16);
}

GameObjectManager::~GameObjectManager() {
    _PickupItems.clear();
    _Cars.clear();
    _Bullets.clear();
}

void GameObjectManager::load() {
    std::cout << "Loading GameObject textures..." << std::endl;

    GameObjectFactory::load();

    _ExplosionTexture.loadFromFile("Resources/Texture/Animation/explosion.png");
}

void GameObjectManager::update(float frameTime) {

    _Player->update(frameTime);

    if (!_Player->isAlive() && !_Player->isDying()) {
        _FW.getLevelManager().setMoving(false);
        _FW.getSoundManager().getLevelMusic()->stop();
        _FW.advanceToGameState(GameState::GameOver);
    }

    spawnObjects(frameTime);

    checkForCollisions(frameTime);

    switchLane(frameTime);

    deleteAllOffScreenObjects();

    deleteDestroyedCars();

    if (_InBossFight) {
        _Boss->update(frameTime);

        checkPlayerBossCollision();

        checkBossBulletCollision();

        if (_Boss->hasTraffic()) {
            checkBossCarsCollision();
        }
    }

    if (_FW.getOptionsManager().getGameMode() != GameMode::InfEnergy) {
        _Player->drainEnergy(frameTime);
    }
}

void GameObjectManager::switchLane(float frameTime) {
    if (_Cars.size() > 0) {
        if (_TimePassedSwitch + frameTime >= 1.0f / _SwitchLaneFrequency) {
            _TimePassedSwitch += frameTime - 1 / _SwitchLaneFrequency;

            std::shared_ptr<AICar> selectedCar = _Cars[rand() % _Cars.size()];
            selectedCar->switchLaneRandomly();
        } else {
            _TimePassedSwitch += frameTime;
        }
    }
}

void GameObjectManager::checkBossCarsCollision() {
    for (unsigned int i = 0; i < _Cars.size(); i++) {
        if (_Boss->checkForCollision(*_Cars[i])) {
            playExplosion(_Cars[i]->getPosition(), _Cars[i]->getMovement());
            rh::deleteObject(_Cars, i);
        }
    }
}

void GameObjectManager::checkBossBulletCollision() {
    for (unsigned int i = 0; i < _Bullets.size(); i++) {
        if (_Boss->checkForCollision(*_Bullets.at(i)) &&
            _Bullets.at(i)->getType() == GameObjectType::BulletPlayer) {
            _Boss->takeDamage(_Player->getBulletdamage());
            _FW.getSoundManager().playHitSound(_Boss->getPosition());
            rh::deleteObject(_Bullets, i);
        }
    }
}

void GameObjectManager::checkPlayerBossCollision() {
    if (_Player->checkForCollision(*_Boss)) {
        _Player->kill();
    }
}

void GameObjectManager::deleteDestroyedCars() {
    if (!_InBossFight || (_InBossFight && getBossCar()->hasTraffic())) {
        for (unsigned int i = 0; i < _Cars.size(); i++) {
            if (_Cars.at(i)->getHealth() <= 0) {
                _FW.getLevelManager().addScore(ScoreEvent::DestroyedCar, _Cars.at(i)->getMaxHealth());
                playExplosion(_Cars[i]->getPosition(), _Cars[i]->getMovement());
                rh::deleteObject(_Cars, i);
            }
        }
    }
}

void GameObjectManager::deleteAllOffScreenObjects() {
    deleteOffScreenObjects(_Cars);

    deleteOffScreenObjects(_Bullets);

    deleteOffScreenObjects(_PickupItems);
}

template<typename GameObjectList>
void GameObjectManager::deleteOffScreenObjects(GameObjectList &goList) {
    for (unsigned int i = 0; i < goList.size(); i++) {
        if (goList.at(i)->getPosition().y - goList.at(i)->getHeight() / 2 > SCREENHEIGHT ||
            goList.at(i)->getPosition().y + goList.at(i)->getHeight() / 2 <= 0 ||
            goList.at(i)->getPosition().x + goList.at(i)->getWidth() / 2 <= 0 ||
            goList.at(i)->getPosition().x - goList.at(i)->getWidth() / 2 >= SCREENWIDTH) {
            rh::deleteObject(goList, i);
        }
    }
}

void GameObjectManager::checkForCollisions(float frameTime) {

    checkPlayerForCollisions(frameTime);

    if (!_InBossFight || (_InBossFight && getBossCar()->hasTraffic())) {

        // Check AICars for collision
        for (unsigned int i = 0; i < _Cars.size(); i++) {
            for (unsigned int j = 0; j < _Cars.size(); j++) {
                if (i != j && _Cars.at(i)->getLane() == _Cars.at(j)->getLane() &&
                    _Cars.at(i)->getSpeed() != _Cars.at(j)->getSpeed()) {
                    if (std::abs(_Cars.at(i)->getPosition().y - _Cars.at(j)->getPosition().y) <
                        _Cars.at(i)->getHeight() + 20) {
                        int minSpeed = std::min({_Cars.at(i)->getSpeed(), _Cars.at(j)->getSpeed()});
                        _Cars.at(i)->setSpeed(minSpeed);
                        _Cars.at(j)->setSpeed(minSpeed);
                    }
                }

                if (i != j) {
                    if (_Cars[i]->checkForCollision(*_Cars[j])) {
                        _Cars.at(i)->takeDamage(500);
                        _Cars.at(j)->takeDamage(500);
                    }
                }
            }
            for (unsigned int j = 0; j < _Bullets.size(); j++) {
                if (_Bullets[j]->getType() != GameObjectType::BulletAI &&
                    _Cars.at(i)->checkForCollision(*_Bullets[j])) {
                    if (_Bullets[j]->getType() == GameObjectType::BulletPlayer) {
                        _Cars.at(i)->takeDamage(_Player->getBulletdamage());
                        _FW.getSoundManager().playHitSound(_Bullets[j]->getPosition());
                    } else {
                        _Cars.at(i)->takeDamage(500);
                    }
                    rh::deleteObject(_Bullets, j);
                    break;
                }
            }
        }
    }
}

void GameObjectManager::checkPlayerForCollisions(float frameTime) {
    // Collision with bullets
    for (unsigned int i = 0; i < _Bullets.size(); i++) {
        _Bullets.at(i)->update(frameTime);
        if (_Player->isAlive() && _Player->checkForCollision(*_Bullets.at(i))) {
            switch (_Bullets[i]->getType()) {
                case GameObjectType::BulletAI:
                case GameObjectType::BulletBoss:
                    if (_FW.getOptionsManager().getGameMode() != GameMode::Invincible) {
                        _Player->takeDamage(5);
                    }
                    _FW.getSoundManager().playHitSound(_Player->getPosition());
                    rh::deleteObject(_Bullets, i);
                    break;
                case GameObjectType::Player:
                    break;
                case GameObjectType::AI:
                    break;
                case GameObjectType::BulletPlayer:
                    break;
                case GameObjectType::Canister:
                    break;
                case GameObjectType::Tools:
                    break;
                case GameObjectType::Boss:
                    break;
            }
        }
    }

    // Collision with gameobjects
    for (unsigned int i = 0; i < _PickupItems.size(); i++) {
        _PickupItems.at(i)->update(frameTime);
        if (_Player->isAlive() && _Player->checkForCollision(*_PickupItems.at(i))) {
            switch (_PickupItems[i]->getType()) {
                case GameObjectType::Canister:
                    _Player->addEnergy();
                    rh::deleteObject(_PickupItems, i);
                    break;
                case GameObjectType::Tools:
                    _Player->addHealth();
                    rh::deleteObject(_PickupItems, i);
                    break;
                case GameObjectType::Player:
                    break;
                case GameObjectType::AI:
                    break;
                case GameObjectType::BulletAI:
                    break;
                case GameObjectType::BulletPlayer:
                    break;
                case GameObjectType::BulletBoss:
                    break;
                case GameObjectType::Boss:
                    break;
            }
        }
    }

    if (!_InBossFight || (_InBossFight && getBossCar()->hasTraffic())) {

        // Collision with AICars
        for (unsigned int i = 0; i < _Cars.size(); i++) {
            _Cars.at(i)->update(frameTime, _FW.getLevelManager().getRoadSpeed());
            if (_Player->isAlive() && _Player->checkForCollision(*_Cars.at(i))) {
                if (_FW.getOptionsManager().getGameMode() == GameMode::Invincible) {
                    playExplosion(_Cars[i]->getPosition(), _Cars[i]->getMovement());
                    rh::deleteObject(_Cars, i);
                } else {
                    _Player->kill();
                }
            }
        }
    }
}

bool GameObjectManager::bossIsDead() {
    if (_InBossFight) {
        if (getBossCar()->getHealth() <= 0) {
            _AboutToLevelUp = true;
            if (getBossCar()->isDoneExploding()) {
                _InBossFight = false;
                _AboutToLevelUp = false;
                _Player->resetResources();

                while (_PickupItems.size() != 0) {
                    unsigned int id = 0;
                    rh::deleteObject(_PickupItems, id);
                }
                while (_Cars.size() != 0) {
                    unsigned int id = 0;
                    rh::deleteObject(_Cars, id);
                }
                while (_Bullets.size() != 0) {
                    unsigned int id = 0;
                    rh::deleteObject(_Bullets, id);
                }

                _FW.getLevelManager().addScore(ScoreEvent::DefeatedBoss, 0);

                return true;
            }
        }
    }
    return false;
}

void GameObjectManager::enterBossFight() {
//    std::cout << "BossHP: " << _FW.getLevelManager().getBossHP() << std::endl;
    _Boss = GameObjectFactory::getBossCar(*this, (_FW.getLevelManager().getLevel() - 1) % 4,
                                          (int) _FW.getOptionsManager().getDifficulty(),
                                          _FW.getLevelManager().getBossHP());
    _InBossFight = true;
}

void GameObjectManager::resetGameObjects() {
    _PickupItems.clear();
    _Cars.clear();
    _Bullets.clear();
    _Animations.clear();

    PlayerCarIndex playerCarIndex = PlayerCarIndex::Car1;
    if (_Player != NULL) {
        playerCarIndex = _Player->getPlayerCarIndex();
    }
    _Player = GameObjectFactory::getPlayerCar(*this, playerCarIndex, false);


    // Frequencies
    calculateAllFrequencies();

    _TimePassedCar = 0.0f;
    _TimePassedBullet = 0.0f;
    _TimePassedCanister = 0.0f;
    _TimePassedToolbox = 0.0f;
    _TimePassedSwitch = 0.0f;

    _AboutToLevelUp = false;
    _InBossFight = false;
}

bool GameObjectManager::emptyScreen() {
    _AboutToLevelUp = true;
    if (_Cars.size() == 0) {
        _AboutToLevelUp = false;
        return true;
    }
    return false;
}

void GameObjectManager::spawnObjects(float frameTime) {
    if (!_AboutToLevelUp) {
        spawnAICar(frameTime);

        spawnToolbox(frameTime);

        spawnCanister(frameTime);
    }

    spawnBullet(frameTime);
}

void GameObjectManager::spawnAICar(float frameTime) {
    if (!_InBossFight || (_InBossFight && getBossCar()->hasTraffic())) {
        if (_TimePassedCar + frameTime > 1 / _CarFrequency) {
            _TimePassedCar += frameTime - 1 / _CarFrequency;

            std::shared_ptr<AICar> newAiCar = GameObjectFactory::getAICar(*this, _FW.getLevelManager().getAiHP(),
                                                                          _FW.getLevelManager().getRoadSpeed());

            for (unsigned int i = 0; i < _Cars.size(); i++) {
                if (_Cars.at(i)->getLane() == newAiCar->getLane() &&
                    _Cars.at(i)->getPosition().y < _Cars.at(i)->getHeight() / 2.0f + 20) {
                    return;
                }
            }

            _Cars.push_back(newAiCar);

        } else {
            _TimePassedCar += frameTime;
        }
    }
}

void GameObjectManager::spawnBullet(float frameTime) {
    if (!_InBossFight || (_InBossFight && getBossCar()->hasTraffic())) {
        if (_Player->isAlive() && _TimePassedBullet + frameTime > 1 / _BulletFrequency) {
            _TimePassedBullet += frameTime - 1 / _BulletFrequency;
            if (_Cars.size() == 0)
                return;

            std::shared_ptr<GameObject> selectedCar = _Cars.at(std::rand() % _Cars.size());

            sf::Vector2f dir = _Player->getPosition() - selectedCar->getPosition();
            shootBullet(GameObjectType::BulletAI, selectedCar->getPosition(), dir, _AIBulletSpeed);

            // FIXME should we really recalculate the freq after every spawn?
            calculateBulletFrequency();
        } else {
            _TimePassedBullet += frameTime;
        }
    }
}

void GameObjectManager::spawnToolbox(float frameTime) {
    _TimePassedToolbox += frameTime;
    if (_TimePassedToolbox > 1.0f / _ToolboxFrequency &&
        _FW.getOptionsManager().getGameMode() != GameMode::Invincible) {
        _TimePassedToolbox -= 1.0f / _ToolboxFrequency;
        _PickupItems.push_back(GameObjectFactory::getToolbox(*this, sf::Vector2f(std::rand() % 3 * 150 + 150, -10),
                                                             _FW.getLevelManager().getRoadSpeed()));

        // FIXME should we really recalculate the freq after every spawn?
        calculateToolboxFrequency();
    }
}

void GameObjectManager::spawnCanister(float frameTime) {
    _TimePassedCanister += frameTime;
    if (_TimePassedCanister > 1.0f / _CanisterFrequency) {
        _TimePassedCanister -= 1.0f / _CanisterFrequency;
        _PickupItems.push_back(GameObjectFactory::getCanister(*this, sf::Vector2f(std::rand() % 3 * 150 + 150, -20),
                                                              _FW.getLevelManager().getRoadSpeed()));
    }
}

void GameObjectManager::calculateAllFrequencies() {
    //TODO scale with difficulty
    _SwitchLaneFrequency = 0.5f;

    calculateAiCarFrequency();
    calculateBulletFrequency();
    calculateCanisterFrequency();
    calculateToolboxFrequency();
}

void GameObjectManager::calculateAiCarFrequency() {
    switch (_FW.getOptionsManager().getDifficulty()) {
        case Difficulty::Easy:
            _CarFrequency = 1.5f + 0.1f * (float) _FW.getLevelManager().getLevel();
            break;
        case Difficulty::Normal:
            _CarFrequency = 1.75f + 0.11f * std::pow((float) _FW.getLevelManager().getLevel(), 1.3f);
            break;
        case Difficulty::Hard:
            _CarFrequency = 2.0f + 0.15f * std::pow((float) _FW.getLevelManager().getLevel(), 1.3f);
            break;
        case Difficulty::Insane:
            _CarFrequency = 2.15f + 0.17f * std::pow((float) _FW.getLevelManager().getLevel(), 1.45f);
            break;
    }
}

void GameObjectManager::calculateBulletFrequency() {
    switch (_FW.getOptionsManager().getDifficulty()) {
        case Difficulty::Easy:
            _BulletFrequency = 0.8f + 0.065f * (float) _FW.getLevelManager().getLevel();
            break;
        case Difficulty::Normal:
            _BulletFrequency = 1.2f + 0.08f * std::pow((float) _FW.getLevelManager().getLevel(), 1.1f);
            break;
        case Difficulty::Hard:
            _BulletFrequency = 1.2f + 1.0f * std::pow((float) _FW.getLevelManager().getLevel(), 1.2f);
            break;
        case Difficulty::Insane:
            _BulletFrequency = 1.4f + 1.0f * std::pow((float) _FW.getLevelManager().getLevel(), 1.33f);
            break;
    }
}

void GameObjectManager::calculateCanisterFrequency() {
    switch (_FW.getOptionsManager().getDifficulty()) {
        case Difficulty::Easy:
            _CanisterFrequency = 0.5f;
            break;
        case Difficulty::Normal:
            _CanisterFrequency = 0.4f;
            break;
        case Difficulty::Hard:
            _CanisterFrequency = 0.3f;
            break;
        case Difficulty::Insane:
            _CanisterFrequency = 0.3f;
            break;
    }
}

void GameObjectManager::calculateToolboxFrequency() {
    switch (_FW.getOptionsManager().getDifficulty()) {
        case Difficulty::Easy:
            _ToolboxFrequency = (float) (std::rand() % 45) / 1000.f + 0.080f;
            break;
        case Difficulty::Normal:
            _ToolboxFrequency = (float) (std::rand() % 20) / 1000.f + 0.060f;
            break;
        case Difficulty::Hard:
            _ToolboxFrequency = (float) (std::rand() % 20) / 1000.f + 0.040f;
            break;
        case Difficulty::Insane:
            _ToolboxFrequency = (float) (std::rand() % 20) / 1000.f + 0.020f;
            break;
    }
}

void GameObjectManager::nextPlayerCar() {
    int index = (int) _Player->getPlayerCarIndex();
    index++;
    if (index >= (int) PlayerCarIndex::NumberOfCars) {
        index = 0;
    }
    _Player = GameObjectFactory::getPlayerCar(*this, (PlayerCarIndex) index, false);
}

void GameObjectManager::previousPlayerCar() {
    int index = (int) _Player->getPlayerCarIndex();
    index--;
    if (index < 0) {
        index = (int) PlayerCarIndex::NumberOfCars - 1;
    }
    _Player = GameObjectFactory::getPlayerCar(*this, (PlayerCarIndex) index, false);
}

void GameObjectManager::shootBullet(GameObjectType type, sf::Vector2f pos, sf::Vector2f dir, int speed) {
    if (type == GameObjectType::BulletPlayer || type == GameObjectType::BulletBoss ||
        type == GameObjectType::BulletAI) {
        dir = rh::normalize(dir);
        _Bullets.push_back(GameObjectFactory::getBullet(*this, pos, dir, speed, type));
        _FW.getSoundManager().playShotSound(type, pos);
    }
}

std::shared_ptr<Explosion> GameObjectManager::playExplosion(sf::Vector2f pos, sf::Vector2f movement) {
    std::shared_ptr<Explosion> newExplosion = std::make_shared<Explosion>(pos, _ExplosionTexture, movement);
    _Animations.push_back(newExplosion);
    _FW.getSoundManager().playExplosionSound(_Player->getPosition());
    return newExplosion;
}
