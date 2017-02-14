#include "stdafx.h"
#include "GameObject/Boss/BossCar.h"

BossCar::BossCar(unsigned int id, sf::Vector2f pos, int difficulty, int Health, float Speed, sf::Texture &texture,
                 sf::Texture &bulletTexture) :
        Car(id, pos, Health, Speed, GameObjectType::Boss, texture,
            sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y)),
        _Difficulty((sf::Uint16) difficulty), _BulletSpeed(500), _BulletTexture(bulletTexture),
        _Movement(Movement::STILL),
        _Attack(false), _HasTraffic(false), _IsExploding(false),
        _Event1Counter(0), _Event2Counter(0), _Event1Frequency(0), _Event2Frequency(0), _Event1Switch(false),
        _Event2Switch(false), _CurrentPhase(0) {
    initBoss();
}

BossCar::BossCar(sf::Packet &packet, sf::Texture &texture, sf::Texture &bulletTexture) :
        Car(packet, GameObjectType::Boss, texture,
            sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y)), _BulletSpeed(500),
        _BulletTexture(bulletTexture), _Movement(Movement::STILL), _Attack(false), _HasTraffic(false),
        _IsExploding(false),
        _Event1Counter(0), _Event2Counter(0), _Event1Frequency(0), _Event2Frequency(0), _Event1Switch(false),
        _Event2Switch(false), _CurrentPhase(0) {
    BossCar::operator<<(packet);
    initBoss();
}

float BossCar::PlayerAngle(GameObject &Player) {
    float Angle;
    sf::Vector2f dir = Player.getPos() - getPos();

    Angle = std::atan(dir.y / dir.x) * 180.0f / PI;
    if (dir.x < 0)
        Angle += 180.0f;

    return Angle;
}

int BossCar::getBossEvent() {
    if (_Event1Frequency != 0 && _BossEventTimer1.getElapsedTime().asSeconds() > 1 / _Event1Frequency) {
        _BossEventTimer1.restart();
        return 1;
    } else if (_Event2Frequency != 0 && _BossEventTimer2.getElapsedTime().asSeconds() > 1 / _Event2Frequency) {
        _BossEventTimer2.restart();
        return 2;
    } else {
        return 0;
    }
}

bool BossCar::driveToNextPosition(float frameTime) {
	float margin = 0.5f * frameTime * (float)_Speed;

    if (std::abs((getPos().y - _NextPosition.y)) < margin && std::abs((getPos().x - _NextPosition.x)) < margin) {
        return true;
    } else {
        sf::Vector2f movement = sf::Vector2f(_NextPosition.x - getPos().x, _NextPosition.y - getPos().y);
        float length = std::sqrt(std::pow(movement.x, 2) + std::pow(movement.y, 2));
        movement = movement / length;

        setPos(getPos() + movement * frameTime * (float) _Speed);

        return false;
    }
}

void BossCar::updateHealthBar() {
    _HealthBar.setPosition(sf::Vector2f(getPos().x - getWidth() / 2 - (_HealthBarFrame.getSize().x - getWidth()) / 2,
                                        getPos().y - getHeight() / 2 - _HealthBarFrame.getSize().y - 8));
    _HealthBar.setSize(
            sf::Vector2f(_HealthBarFrame.getSize().x * getHealth() / getMaxHealth(), _HealthBarFrame.getSize().y));
    _HealthBarFrame.setPosition(_HealthBar.getPosition());
}

void BossCar::checkPhase() {
    if (_PhaseClock.getElapsedTime().asSeconds() > _Pattern[_CurrentPhase].second) {
        if (_CurrentPhase + 1 >= _Pattern.size()) {
            _CurrentPhase = 0;
        } else {
            _CurrentPhase++;
        }

        _BossEventTimer1.restart();
        _BossEventTimer2.restart();
        _Event1Switch = false;
        _Event2Switch = false;
        _Event2Counter = 0;
        _Event1Counter = 0;

        _PhaseClock.restart();
    }
}

void BossCar::renderExplosions(sf::RenderWindow &window) {
    if (_IsExploding) {
        for (int i = 0; i < _Explosions.size(); i++) {
            _Explosions[i]->render(window);
        }
    }
}

void BossCar::updateExplosions(float frameTime) {
    for (int i = 0; i < _Explosions.size(); i++) {
        _Explosions[i]->update(frameTime);
    }
}

bool BossCar::isDoneExploding(sf::Texture &explosionTexture) {
    if (_Health <= 0 && !(bool) _IsExploding) {
        _IsExploding = 1;
        _ExplosionTimer.restart();
    }
    if (_Health <= 0 && _IsExploding && _ExplosionTimer.getElapsedTime().asSeconds() > 0.2) {
        _ExplosionTimer.restart();
        sf::Vector2f position = sf::Vector2f(0, 0);
        switch (_Explosions.size()) {
            case 1:
                position = sf::Vector2f(getWidth() / 3.0f, getHeight() / 3.0f);
                break;
            case 2:
                position = sf::Vector2f(getWidth() / 3.0f, getHeight() / -3.0f);
                break;
            case 3:
                position = sf::Vector2f(getWidth() / -3.0f, getHeight() / -3.0f);
                break;
            case 4:
                position = sf::Vector2f(getWidth() / -3.0f, getHeight() / 3.0f);
                break;
        }
        _Explosions.push_back(new Explosion(getPos() + position, explosionTexture, sf::Vector2f(0, 0)));
        // FIXME play explosion sound (maybe move creation of explosion somewhere else?)
    }
    if (_Explosions.size() > 5) {
        _IsExploding = 2;
    }
    return (_Health <= 0 && _IsExploding == 2);
}

sf::Vector2f BossCar::calcBulletPosition() {
    return getPos() + _GunPosition + _GunOrientation * _GunLength;
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
    _BossEventTimer1.restart();
    _BossEventTimer2.restart();
    _PhaseClock.restart();

    _Pattern = {std::make_pair(Phase::NOTHING, 1.0f)};

    //HP-Balken
    _HealthBar.setFillColor(sf::Color(200, 0, 0));
    _HealthBar.setSize(sf::Vector2f(getWidth() - 1, 5));

    _HealthBarFrame.setFillColor(sf::Color::Transparent);
    _HealthBarFrame.setOutlineColor(sf::Color(20, 0, 0));
    _HealthBarFrame.setOutlineThickness(1);
    _HealthBarFrame.setSize(_HealthBar.getSize());
}
