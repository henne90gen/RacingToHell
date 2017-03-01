#include "stdafx.h"
#include "GameObject/Boss/Carrier.h"

Carrier::Carrier(unsigned int id, GameObjectManager &gom, int difficulty, int HP, sf::Texture &texture,
                 sf::Texture &bulletTexture) :
        BossCar(id, gom, sf::Vector2f(SCREENWIDTH / 2, -1 * (float) texture.getSize().y / 2.0f), difficulty, HP, 2000,
                texture, bulletTexture),
        _MovementSwitchLeftRight(false), _MovementSwitchUpDown(false), _Radius(50), _SwitchSideTime(8.0f),
        _SwitchSides(false) {
    init();
}

//Carrier::Carrier(sf::Packet &packet, sf::Texture &texture, sf::Texture &bulletTexture, PlayerCar &player) :
//        BossCar(packet, texture, bulletTexture, player) {
//    init();
//}

void Carrier::render(sf::RenderWindow &window) {
    window.draw(getSprite());
    window.draw(_GunSprite);

    window.draw(_HealthBar);
    window.draw(_HealthBarFrame);

    renderExplosions(window);
}

void Carrier::init() {
    _GunTexture.loadFromFile("Resources/Texture/BossCar/CannonCarrier.png");
    _GunSprite.setTexture(_GunTexture);
    _GunSprite.setOrigin(_GunTexture.getSize().x / 2, 50);

    _GunPosition = sf::Vector2f(0, 0);
    _GunLength = _GunSprite.getLocalBounds().height - 50;

    _DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, 150);
    _NextPosition = _DefaultPosition;

//    _MovementCommand = Movement::DRIVETODEFAULT;
//    _Pattern = {std::make_pair(Phase::BLASTSALVE, 5.0f), std::make_pair(Phase::NOTHING, 1.5f),
//                std::make_pair(Phase::RANDOMSPRAY, 6.0f), std::make_pair(Phase::NOTHING, 1.5f),
//                std::make_pair(Phase::SPIRAL, 8.0f), std::make_pair(Phase::NOTHING, 2.0f),
//                std::make_pair(Phase::HARDCORESPAM, 7.0f), std::make_pair(Phase::NOTHING, 1.5f)};
}

void Carrier::update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<Bullet>> &bullets) {
    Car::update(frameTime);

//    if (!_IsExploding) {
//        if (driveToNextPosition(frameTime)) {
//            switch (_MovementCommand) {
//                case Movement::DRIVETODEFAULT:
//                    _MovementCommand = Movement::LEFTRIGHT;
//                    _Speed = 200;
//                    _Attack = true;
//                    _PhaseClock.restart();
//                    _SwitchSidesClock.restart();
//                    break;
//                case Movement::LEFTRIGHT:
//                    _Attack = true;
//                    _MovementSwitchLeftRight = !_MovementSwitchLeftRight;
//
//                    if (_MovementSwitchLeftRight) {
//                        _NextPosition = getPosition() + sf::Vector2f(
//                                (SCREENWIDTH - getPosition().x - getWidth() / 2) * (std::rand() % 100) / 100.0f, 0.0f);
//                    } else {
//                        _NextPosition = getPosition() -
//                                        sf::Vector2f((getPosition().x - getWidth() / 2) * (std::rand() % 100) / 100.0f,
//                                                     0.0f);
//                    }
//                    break;
//                case Movement::SWITCHSIDES:
//                    _MovementCommand = Movement::LEFTRIGHT;
//                    _MovementSwitchUpDown = !_MovementSwitchUpDown;
//                    _Attack = true;
//                    _Speed = 200;
//                    _SwitchSidesClock.restart();
//                    _BossEventTimer1.restart();
//                    _BossEventTimer2.restart();
//                    _Event1Switch = false;
//                    _Event2Switch = false;
//                    _Event2Counter = 0;
//                    _Event1Counter = 0;
//
//                    _PhaseClock.restart();
//
//                    break;
//                case STILL:
//                    break;
//                case STRAIGHT:
//                    break;
//                case PARABOLA:
//                    break;
//                case RUNATPLAYER:
//                    break;
//                case ZICKZACK:
//                    break;
//            }
//
//            //if (_SwitchSidesClock.getElapsedTime().asSeconds() >= _SwitchSideTime)
//
//        }
//
//        if (_SwitchSides) {
//            _MovementCommand = Movement::SWITCHSIDES;
//            _NextPosition = sf::Vector2f(getWidth() / 2 + (SCREENWIDTH - getWidth()) * (std::rand() % 100) / 100,
//                                         ((int) (!_MovementSwitchUpDown) * (SCREENHEIGHT - 2 * _DefaultPosition.y)) +
//                                         _DefaultPosition.y);
//            _Speed = 450;
//            _Attack = false;
//            _SwitchSides = false;
//        }
//
//        if (_MovementCommand == Movement::SWITCHSIDES) {
//            getSprite().setRotation((getPosition().y - _DefaultPosition.y) * 180 / (SCREENHEIGHT - 2 * _DefaultPosition.y));
//        }
//
//        _GunOrientation = rh::normalize(player.getPosition() - getPosition());
//
//        if (_Attack) {
//            switch (_Pattern[_CurrentPhase].first) {
//                case Phase::BLASTSALVE:
//                    _Event1Frequency = 0.4f;
//                    _Event2Frequency = 7.0f;
//
//                    if (_Event1Switch) {
//                        if (getBossEvent() == 2) {
//                            _Event2Counter = 0;
//                            for (float i = (2 * _Event1Counter) * PI / 180;
//                                 i <= 2 * PI; i += PI / (7 + 2 * _Difficulty)) {
//                                _Event2Counter++;
//                                sf::Vector2f orientation = sf::Vector2f(std::cos(i), std::sin(i));
//
//								BossCar::shootBullet(bullets, calcBulletPosition(), _GunOrientation);
//                            }
//
//                            if (_Event1Counter + 1 < 4 + _Difficulty) {
//                                _Event1Counter += 1;
//                            } else {
//                                _Event1Switch = false;
//                                _Event1Counter = 0;
//                            }
//                        }
//                    } else {
//                        if (getBossEvent() == 1) {
//                            _Event1Switch = true;
//                        }
//                    }
//                    break;
//                case Phase::SPIRAL:
//                    _Event1Frequency = 3.0f;
//                    _Event2Frequency = 10.0f;
//
//                    if (_Event1Switch) {
//                        if (getBossEvent() == 2) {
//                            _Event1Counter = 0;
//                            for (float i = 0.0f; i < 2 * PI; i += PI / (4 + _Difficulty)) {
//                                _Event1Counter++;
//                                _GunOrientation = sf::Vector2f(std::cos(i), std::sin(i));
//
//								BossCar::shootBullet(bullets, calcBulletPosition(), _GunOrientation);
//
//                                _GunOrientation = rh::normalize(player.getPosition() - getPosition());
//                            }
//                        }
//                    } else {
//                        if (getBossEvent() == 1) {
//                            _Event1Switch = true;
//                        }
//                    }
//                    break;
//                case Phase::RANDOMSPRAY:
//                    _Event1Frequency = 0.8f + 0.10f * (float) _Difficulty;
//
//                    if (getBossEvent() == 1) {
//                        for (float i = PI * (float) _MovementSwitchUpDown;
//                             i < PI + PI * (float) _MovementSwitchUpDown; i += PI / (10 + 3 * _Difficulty)) {
//                            _GunOrientation = sf::Vector2f(std::cos(i), std::sin(i));
//                            sf::Vector2f orientation = rh::normalize(
//                                    sf::Vector2f(((std::rand() - (float) (RAND_MAX) / 2) / (float) (RAND_MAX)),
//                                                 std::rand() / (float) (RAND_MAX) *
//                                                 std::pow(-1, (int) (_MovementSwitchUpDown))));
//                            BossCar::shootBullet(bullets, calcBulletPosition(), orientation);
//                        }
//                    }
//                    break;
//
//                case Phase::HARDCORESPAM:
//                    _Event1Frequency = 40.0f + 15.0f * (float) _Difficulty;
//
//                    if (getBossEvent() == 1) {
//                        _Event1Counter++;
//                        _GunOrientation = rh::normalize(
//                                sf::Vector2f(((std::rand() - (float) (RAND_MAX) / 2) / (float) (RAND_MAX)),
//                                             ((std::rand() - (float) (RAND_MAX) / 2) / (float) (RAND_MAX))));
//						BossCar::shootBullet(bullets, calcBulletPosition(), _GunOrientation);
//                    }
//                    break;
//            }
//        }
//
//        _GunSprite.setPosition(getPosition() + _GunPosition);
//        _GunSprite.setRotation(rh::getAngleFromVector(_GunOrientation) - 90);
//
//        updateHealthBar();
//        checkPhase();
//    } else {
//        updateExplosions(frameTime);
//    }
}

void Carrier::checkPhase() {
//    if (_PhaseClock.getElapsedTime().asSeconds() > _Pattern[_CurrentPhase].second) {
//        if (_Pattern[_CurrentPhase].first != Phase::NOTHING && std::rand() % RAND_MAX > 0.5f * RAND_MAX) {
//            _SwitchSides = true;
//        }
//
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
