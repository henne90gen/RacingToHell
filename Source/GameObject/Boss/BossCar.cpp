#include "stdafx.h"
#include <GameObject/Boss/Action/BossAction.h>
#include "GameObject/Boss/BossCar.h"
#include "Framework/GameObjectManager.h"

BossCar::BossCar(unsigned int id, GameObjectManager &gom, sf::Vector2f pos, int difficulty, int Health, float Speed,
                 sf::Texture &texture, sf::Texture &bulletTexture) :
        Car(id, gom, pos, Health, Speed, GameObjectType::Boss, texture,
            sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y)),
        _Difficulty((sf::Uint16) difficulty),
        _BulletSpeed(500),
        _BulletTexture(bulletTexture),
        _Attack(false),
        _HasTraffic(false),
        _IsExploding(false)
//        _MovementCommand(Movement::STILL),
//        _Event1Counter(0), _Event2Counter(0), _Event1Frequency(0), _Event2Frequency(0), _Event1Switch(false),
//        _Event2Switch(false), _CurrentPhase(0)
{
    initBoss();
}

//BossCar::BossCar(sf::Packet &packet, sf::Texture &texture, sf::Texture &bulletTexture, PlayerCar &player) :
//        Car(packet, GameObjectType::Boss, texture,
//            sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y)), _BulletSpeed(500),
//        _BulletTexture(bulletTexture), _Attack(false), _HasTraffic(false),
//        _IsExploding(false), _Player(player)
//        _MovementCommand(Movement::STILL),
//        _Event1Counter(0), _Event2Counter(0), _Event1Frequency(0), _Event2Frequency(0), _Event1Switch(false),
//        _Event2Switch(false), _CurrentPhase(0)
//{
//    BossCar::operator<<(packet);
//    initBoss();
//}

float BossCar::playerAngle(GameObject &Player) {
    float angle;
    sf::Vector2f dir = Player.getPosition() - getPosition();

    angle = std::atan(dir.y / dir.x) * 180.0f / PI;
    if (dir.x < 0) {
        angle += 180.0f;
    }
    return angle;
}

int BossCar::getBossEvent() {
//    if (_Event1Frequency != 0 && _BossEventTimer1.getElapsedTime().asSeconds() > 1 / _Event1Frequency) {
//        _BossEventTimer1.restart();
//        return 1;
//    } else if (_Event2Frequency != 0 && _BossEventTimer2.getElapsedTime().asSeconds() > 1 / _Event2Frequency) {
//        _BossEventTimer2.restart();
//        return 2;
//    } else {
//        return 0;
//    }
}

bool BossCar::driveToNextPosition(float frameTime) {
    float margin = 0.5f * frameTime * (float) _Speed;

    if (std::abs((getPosition().y - _NextPosition.y)) < margin &&
        std::abs((getPosition().x - _NextPosition.x)) < margin) {
        return true;
    } else {
        sf::Vector2f movement = sf::Vector2f(_NextPosition.x - getPosition().x, _NextPosition.y - getPosition().y);
        float length = std::sqrt(std::pow(movement.x, 2) + std::pow(movement.y, 2));
        movement = movement / length;

        setPos(getPosition() + movement * frameTime * (float) _Speed);

        return false;
    }
}

void BossCar::updateHealthBar() {
    _HealthBar.setPosition(sf::Vector2f(
            getPosition().x - getWidth() / 2 - (_HealthBarFrame.getSize().x - getWidth()) / 2,
            getPosition().y - getHeight() / 2 - _HealthBarFrame.getSize().y - 8));
    _HealthBar.setSize(
            sf::Vector2f(_HealthBarFrame.getSize().x * getHealth() / getMaxHealth(), _HealthBarFrame.getSize().y));
    _HealthBarFrame.setPosition(_HealthBar.getPosition());
}

void BossCar::checkPhase() {
//    if (_PhaseClock.getElapsedTime().asSeconds() > _Pattern[_CurrentPhase].second) {
//        if (_CurrentPhase + 1 >= _Pattern.size()) {
//            _CurrentPhase = 0;
//        } else {
//            _CurrentPhase++;
//        }
//
//        _BossEventTimer1.restart();
//        _BossEventTimer2.restart();
//        _Event1Switch = false;
//        _Event2Switch = false;
//        _Event2Counter = 0;
//        _Event1Counter = 0;
//
//        _PhaseClock.restart();
//    }
}

void BossCar::updateExplosions(float frameTime) {
    for (unsigned int i = 0; i < _Explosions.size(); i++) {
        if (_Explosions[i]->getAnimationState() == Animation::AnimationState::Stop) {
            rh::deleteObject(_Explosions, i);
        }
    }
}

bool BossCar::isDoneExploding() {
    if (_Health <= 0 && !(bool) _IsExploding) {
        _IsExploding = 1;
        _ExplosionTimer.restart();
    }
    if (_Health <= 0 && _IsExploding && _ExplosionTimer.getElapsedTime().asSeconds() > 0.2) {
        _ExplosionTimer.restart();
        sf::Vector2f offset = sf::Vector2f(0, 0);
        switch (_Explosions.size()) {
            case 1:
                offset = sf::Vector2f(getWidth() / 3.0f, getHeight() / 3.0f);
                break;
            case 2:
                offset = sf::Vector2f(getWidth() / 3.0f, getHeight() / -3.0f);
                break;
            case 3:
                offset = sf::Vector2f(getWidth() / -3.0f, getHeight() / -3.0f);
                break;
            case 4:
                offset = sf::Vector2f(getWidth() / -3.0f, getHeight() / 3.0f);
                break;
            default:
                break;
        }
        _Explosions.push_back(_GOM.playExplosion(getPosition() + offset, sf::Vector2f(0, 0)));
    }
    if (_Explosions.size() > 5) {
        _IsExploding = 2;
    }
    return (_Health <= 0 && _IsExploding == 2);
}

void BossCar::operator>>(sf::Packet &packet) {
    Car::operator>>(packet);
    write(packet, _Difficulty);
}

void BossCar::operator<<(sf::Packet &packet) {
    Car::operator<<(packet);
    read(packet, _Difficulty);
}

void BossCar::initBoss() {
//    _BossEventTimer1.restart();
//    _BossEventTimer2.restart();
//    _PhaseClock.restart();

//    _Pattern = {std::make_pair(Phase::NOTHING, 1.0f)};

    //HP-Balken
    _HealthBar.setFillColor(sf::Color(200, 0, 0));
    _HealthBar.setSize(sf::Vector2f(getWidth() - 1, 5));
    sf::Vector2f origin = sf::Vector2f(_HealthBar.getLocalBounds().width / 2, getHeight() / 2);
    _HealthBar.setOrigin(origin);

    _HealthBarFrame.setFillColor(sf::Color::Transparent);
    _HealthBarFrame.setOutlineColor(sf::Color(20, 0, 0));
    _HealthBarFrame.setOutlineThickness(1);
    _HealthBarFrame.setSize(_HealthBar.getSize());
    _HealthBarFrame.setOrigin(origin);
}

void BossCar::update(float frameTime) {
//    std::cout << "BossPos: " << _Position.x << ", " << _Position.y << " | " << _Actions.size() << std::endl;

    bool doneExecuting = true;
    for (unsigned int i = 0; i < _Actions.size(); i++) {
        _Actions.at(i)->execute();
        if (!_Actions.at(i)->hasBeenExecuted()) {
            doneExecuting = false;
        } else if (_Actions[i]->deleteAfterExecution()) {
            rh::deleteObject(_Actions, i);
        }
    }

    if (doneExecuting) {
        updateActions();
    }

    Car::update(frameTime);

    _GunDirection = rh::rotateVector(_GunDirection, _GunMovement * _GunSpeed * frameTime);

    updateHealthBar();

    // TODO update health bar length
//    _HealthBar.setPosition(getPosition());
//    _HealthBarFrame.setPosition(getPosition());
}

void BossCar::shootBullet(sf::Vector2f pos, sf::Vector2f dir) {
    _GOM.shootBullet(GameObjectType::BulletBoss, pos, dir, _BulletSpeed);
}

sf::Vector2f BossCar::getGunDirection() {
    return _GunDirection;
}

sf::Vector2f BossCar::getGunEnd() {
    return getPosition() + _GunOffset + _GunDirection * _GunLength;
}

void BossCar::rotateGunTowards(sf::Vector2f newDirection) {
    float angle = rh::angleBetweenVectors(_GunDirection, newDirection);
    // Scaling angle up to let integer rounding take care of setting movement to 0 when the angle is too small
    // This value has been found with trial and error
    // Anything bigger is inaccurate and anything smaller lets the gun move back and forth constantly
    angle *= 25;
    setGunMovement((int) angle);
}

void BossCar::setGunMovement(int movement) {
    if (movement > 0) {
        _GunMovement = 1;
    } else if (movement < 0) {
        _GunMovement = -1;
    } else {
        _GunMovement = 0;
    }
}
