#include "stdafx.h"
#include "Framework/GameObjectManager.h"
#include "Framework/Framework.h"

GameObjectManager::GameObjectManager(Framework &framework) : _FW(framework), _PlayerBulletSpeed(600),
                                                             _AIBulletSpeed(400), _BossFight(false) {
    _CarFrequency = 1;
    _CanisterFrequency = 0.5;
    _ToolboxFrequency = 0.25;
    _BulletFrequency = 2;
}

GameObjectManager::~GameObjectManager() {
    _PickupItems.clear();
    _Cars.clear();
    _Bullets.clear();
}

void GameObjectManager::update(float frameTime) {

    _Player->update(frameTime, _FW.getLevelManager().getRoadSpeed());

    if (!_Player->isAlive() && !_Player->isDying()) {
        _FW.getLevelManager().stopMoving();
        _FW.advanceToGamState(GameState::GameOver);
    }

    spawnObjects(frameTime);

    checkForCollisions(frameTime);

    deleteAllOffScreenObjects();

    deleteDestroyedCars();

    // TODO Activate BossFight again
    /*
    if (_BossFight) {
        _Boss->update(frameTime, _FW.getLevelManager().getRoadSpeed(), _Bullets, *_Player);

        // Collision player with boss
        if (_Player->checkForCollision(*_Boss)) {
            _Animations.push_back(std::shared_ptr<Explosion>(
                    new Explosion(_Player->getPos(), _ExplosionTexture, sf::Vector2f(0, 0), _SoundEffects,
                                  _ExplosionSoundBuffer, _FW.getOptionsManager().getVolume())));
            _PlayerAlive = false;
        }
        for (unsigned int i = 0; i < _Bullets.size(); i++) {
            if (_Boss->checkForCollision(*_Bullets.at(i)) &&
                _Bullets.at(i)->getType() == GameObjectType::BulletObjectPlayer) {
                _Boss->takeDamage(_Player->getBulletdamage());
                playHitSound(_Boss->getPos());
                deleteObject(_Bullets, i);
                i--;
            }
        }
        for (unsigned int i = 0; i < _Cars.size(); i++) {
            if (_Boss->checkForCollision(*_Cars[i])) {
                std::shared_ptr<Explosion> newExplosion(
                        new Explosion(_Cars.at(i)->getPos(), _ExplosionTexture, sf::Vector2f(0, _Cars[i]->getSpeed()),
                                      _SoundEffects, _ExplosionSoundBuffer, _FW.getOptionsManager().getVolume()));
                _Animations.push_back(newExplosion);
                deleteObject(_Cars, i);
                i--;
            }
        }
    }
    */

    if (_FW.getOptionsManager().getGameMode() != GameMode::InfEnergy) {
        _Player->drainEnergy(frameTime);
    }

    // Check whether player fired a shot
    sf::Vector2f bulletDir = _Player->getShotBullet();
    if ((bulletDir.x != 0 || bulletDir.y != 0) &&
        (_FW.getOptionsManager().getGameMode() == GameMode::InfEnergy || _Player->drainShotEnergy())) {
        _Bullets.push_back(GameObjectFactory::getBullet(_Player->getPos(), bulletDir, _PlayerBulletSpeed,
                                                        GameObjectType::BulletObjectPlayer));
        _FW.getSoundManager().playShotSound(GameObjectType::Player, _Player->getPos());
    }
}

void GameObjectManager::deleteDestroyedCars() {
    if (!_BossFight || (_BossFight && getBossCar()->hasTraffic())) {
        for (unsigned int i = 0; i < _Cars.size(); i++) {
            if (_Cars.at(i)->getHealth() == 0) {
                _FW.getLevelManager().addScore(ScoreEvent::DestroyedCar, _Cars.at(i)->getMaxHealth());
                std::shared_ptr<Explosion> newExplosion(
                        new Explosion(_Cars.at(i)->getPos(), _ExplosionTexture, sf::Vector2f(0, _Cars[i]->getSpeed())));
                _FW.getSoundManager().playExplosionSound(_Cars.at(i)->getPos());
                _Animations.push_back(newExplosion);
                deleteObject(_Cars, i);
                i--;
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
        if (goList.at(i)->getPos().y - goList.at(i)->getHeight() / 2 > SCREENHEIGHT ||
            goList.at(i)->getPos().y + goList.at(i)->getHeight() / 2 <= 0 ||
            goList.at(i)->getPos().x + goList.at(i)->getWidth() / 2 <= 0 ||
            goList.at(i)->getPos().x - goList.at(i)->getWidth() / 2 >= SCREENWIDTH) {
            deleteObject(goList, i);
            i--;
        }
    }
}

void GameObjectManager::checkForCollisions(float frameTime) {

    checkPlayerForCollisions(frameTime);

    if (!_BossFight || (_BossFight && getBossCar()->hasTraffic())) {

        // Check AICars for collision
        for (unsigned int i = 0; i < _Cars.size(); i++) {
            for (unsigned int j = 0; j < _Cars.size(); j++) {
                if (i != j && _Cars.at(i)->getLane() == _Cars.at(j)->getLane() &&
                    _Cars.at(i)->getSpeed() != _Cars.at(j)->getSpeed()) {
                    if (std::abs(_Cars.at(i)->getPos().y - _Cars.at(j)->getPos().y) < _Cars.at(i)->getHeight() + 20) {
                        int minSpeed = std::min({_Cars.at(i)->getSpeed(), _Cars.at(j)->getSpeed()});
                        _Cars.at(i)->setSpeed(minSpeed);
                        _Cars.at(j)->setSpeed(minSpeed);
                    }
                }
            }
            for (unsigned int j = 0; j < _Bullets.size(); j++) {
                if (_Bullets[j]->getType() != GameObjectType::BulletObjectAI &&
                    _Cars.at(i)->checkForCollision(*_Bullets[j])) {
                    if (_Bullets[j]->getType() == GameObjectType::BulletObjectPlayer) {
                        _Cars.at(i)->takeDamage(_Player->getBulletdamage());
                        _FW.getSoundManager().playHitSound(_Bullets[j]->getPos());
                    } else {
                        _Cars.at(i)->takeDamage(500);
                    }
                    deleteObject(_Bullets, j);
                    break;
                }
            }
        }
    }
}

void GameObjectManager::checkPlayerForCollisions(float frameTime) {
    // Collision with bullets
    for (unsigned int i = 0; i < _Bullets.size(); i++) {
        _Bullets.at(i)->update(frameTime, _FW.getLevelManager().getRoadSpeed());
        if (_Player->isAlive() && _Player->checkForCollision(*_Bullets.at(i))) {
            switch (_Bullets[i]->getType()) {
                case GameObjectType::BulletObjectAI:
                case GameObjectType::BulletObjectBoss:
                    if (_FW.getOptionsManager().getGameMode() != GameMode::Invincible) {
                        _Player->takeDamage(5);
                        if (!_Player->isAlive()) {
                            killPlayer();
                        }
                    }
                    _FW.getSoundManager().playHitSound(_Player->getPos());
                    deleteObject(_Bullets, i);
                    i--;
                    break;
            }
        }
    }

    // Collision with gameobjects
    for (unsigned int i = 0; i < _PickupItems.size(); i++) {
        _PickupItems.at(i)->update(frameTime, _FW.getLevelManager().getRoadSpeed());
        if (_Player->isAlive() && _Player->checkForCollision(*_PickupItems.at(i))) {
            switch (_PickupItems[i]->getType()) {
                case GameObjectType::Canister:
                    _Player->addEnergy();
                    deleteObject(_PickupItems, i);
                    i--;
                    break;
                case GameObjectType::Tools:
                    _Player->addHealth();
                    deleteObject(_PickupItems, i);
                    i--;
                    break;
            }
        }
    }

    if (!_BossFight || (_BossFight && getBossCar()->hasTraffic())) {

        // Collision with AICars
        for (unsigned int i = 0; i < _Cars.size(); i++) {
            _Cars.at(i)->update(frameTime, _FW.getLevelManager().getRoadSpeed());
            if (_Player->isAlive() && _Player->checkForCollision(*_Cars.at(i))) {
                // FIXME reenable invincibility mode
//            if (_FW.getOptionsManager().getGameMode() == GameMode::Invincible) {
//                const std::shared_ptr<Explosion> newExplosion(
//                        new Explosion(_Cars.at(i)->getPos(), _ExplosionTexture, sf::Vector2f(0, _Cars[i]->getSpeed()),
//                                      _SoundEffects, _ExplosionSoundBuffer, _FW.getOptionsManager().getVolume()));
//                _Animations.push_back(newExplosion);
//                deleteObject(_Cars, i);
//                i--;
//            } else {
                killPlayer();
//            }
            }
        }
    }
}

void GameObjectManager::killPlayer() {
    _Player->kill(_ExplosionTexture);
    _FW.getSoundManager().playExplosionSound(_Player->getPos());
}

void GameObjectManager::spawnObjects(float frameTime) {

    spawnAICar(frameTime);

    spawnBullet(frameTime);

    spawnCanister(frameTime);

    spawnToolbox(frameTime);
}

void GameObjectManager::stopSounds() {
    if (_BossFight) {
        getBossCar()->stopSounds();
    }
}

bool GameObjectManager::bossIsDead() {
    if (_BossFight) {
        if (getBossCar()->getHealth() <= 0) {
            _AboutToLevelUp = true;
        }
        if (_AboutToLevelUp && getBossCar()->isDoneExploding(_ExplosionTexture)) {
            _BossFight = false;
            _AboutToLevelUp = false;
            _Player->resetResources();

            while (_PickupItems.size() != 0) {
                deleteObject(_PickupItems, 0);
            }
            while (_Cars.size() != 0) {
                deleteObject(_Cars, 0);
            }
            while (_Bullets.size() != 0) {
                deleteObject(_Bullets, 0);
            }

            // FIXME move this to levelmanager::addScore
//            _CarScore =
//                    5000 + 10000 * _FW.getOptionsManager().getDifficulty() * _FW.getOptionsManager().getDifficulty();

            return true;
        }
    }
    return false;
}

void GameObjectManager::enterBossFight() {
    _Boss = GameObjectFactory::getBossCar((_FW.getLevelManager().getLevel() - 1) % 4,
                                          (int) _FW.getOptionsManager().getDifficulty(),
                                          _FW.getLevelManager().getBossHP());
    _BossFight = true;
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
    _Player = GameObjectFactory::getPlayerCar(playerCarIndex);

    // Frequencies
    calculateAllFrequencies();

    _TimePassedCar = 0.0f;
    _TimePassedBullet = 0.0f;
    _TimePassedCanister = 0.0f;
    _TimePassedToolbox = 0.0f;

    _AboutToLevelUp = false;
    _BossFight = false;
}

bool GameObjectManager::emptyScreen() {
    _AboutToLevelUp = true;
    if (_Cars.size() == 0) {
        _AboutToLevelUp = false;
        return true;
    }
    return false;
}

void GameObjectManager::load() {
    std::cout << "Loading object textures..." << std::endl;

    GameObjectFactory::load();

    _ExplosionTexture.loadFromFile("Resources/Texture/Animation/explosion.png");
}

void GameObjectManager::spawnAICar(float frameTime) {
    if (!_BossFight || (_BossFight && getBossCar()->hasTraffic())) {
        if (!_AboutToLevelUp) {
            if (_TimePassedCar + frameTime > 1 / _CarFrequency) {
                _TimePassedCar += frameTime - 1 / _CarFrequency;

                std::shared_ptr<AICar> newAiCar = GameObjectFactory::getAICar(_FW.getLevelManager().getAiHP(),
                                                                              _FW.getLevelManager().getRoadSpeed());

                for (unsigned int i = 0; i < _Cars.size(); i++) {
                    if (_Cars.at(i)->getLane() == newAiCar->getLane() &&
                        _Cars.at(i)->getPos().y < _Cars.at(i)->getHeight() / 2.0f + 20) {
                        return;
                    }
                }

                _Cars.push_back(newAiCar);

            } else {
                _TimePassedCar += frameTime;
            }
        }
    }
}

void GameObjectManager::spawnBullet(float frameTime) {
    if (!_BossFight || (_BossFight && getBossCar()->hasTraffic())) {
        if (_Player->isAlive() && _TimePassedBullet + frameTime > 1 / _BulletFrequency) {
            _TimePassedBullet += frameTime - 1 / _BulletFrequency;
            if (_Cars.size() == 0)
                return;

            std::shared_ptr<GameObject> selectedCar = _Cars.at(std::rand() % _Cars.size());

            sf::Vector2f dir = selectedCar->divideByLength(_Player->getPos() - selectedCar->getPos());

            const std::shared_ptr<Bullet> &newBullet = GameObjectFactory::getBullet(selectedCar->getPos(), dir,
                                                                                    _AIBulletSpeed,
                                                                                    GameObjectType::BulletObjectAI);
            _FW.getSoundManager().playShotSound(GameObjectType::AI, selectedCar->getPos());
            _Bullets.push_back(newBullet);

            // FIXME should we really recalculate the freq after every spawn?
            calculateBulletFrequency();
        } else {
            _TimePassedBullet += frameTime;
        }
    }
}

void GameObjectManager::spawnToolbox(float frameTime) {
    if (!_AboutToLevelUp) {
        _TimePassedToolbox += frameTime;
        if (_TimePassedToolbox > 1.0f / _ToolboxFrequency &&
            _FW.getOptionsManager().getGameMode() != GameMode::Invincible) {
            _TimePassedToolbox -= 1.0f / _ToolboxFrequency;
            _PickupItems.push_back(GameObjectFactory::getToolbox(sf::Vector2f(std::rand() % 3 * 150 + 150, -10)));

            // FIXME should we really recalculate the freq after every spawn?
            calculateToolboxFrequency();
        }
    }
}

void GameObjectManager::spawnCanister(float frameTime) {
    if (!_AboutToLevelUp) {
        _TimePassedCanister += frameTime;
        if (_TimePassedCanister > 1.0f / _CanisterFrequency) {
            _TimePassedCanister -= 1.0f / _CanisterFrequency;
            _PickupItems.push_back(GameObjectFactory::getCanister(sf::Vector2f(std::rand() % 3 * 150 + 150, -20)));
        }
    }
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

void GameObjectManager::calculateAllFrequencies() {
    calculateAiCarFrequency();
    calculateBulletFrequency();
    calculateCanisterFrequency();
    calculateToolboxFrequency();
}

void GameObjectManager::nextPlayerCar() {
    int index = (int) _Player->getPlayerCarIndex();
    index++;
    if (index >= (int) PlayerCarIndex::NumberOfCars) {
        index = 0;
    }
    _Player = GameObjectFactory::getPlayerCar((PlayerCarIndex) index);
}

void GameObjectManager::previousPlayerCar() {
    int index = (int) _Player->getPlayerCarIndex();
    index--;
    if (index < 0) {
        index = (int) PlayerCarIndex::NumberOfCars - 1;
    }
    _Player = GameObjectFactory::getPlayerCar((PlayerCarIndex) index);
}
