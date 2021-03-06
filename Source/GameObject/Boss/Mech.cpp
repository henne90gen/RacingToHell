#include "stdafx.h"
#include "GameObject/Boss/Mech.h"

Mech::Mech(unsigned int id, GameObjectManager &gom, int difficulty, int HP, sf::Texture &textureTop,
           sf::Texture &textureLegs, sf::Texture &bulletTexture) :
        BossCar(id, gom, sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), difficulty, HP, 100, textureTop,
                bulletTexture),
        _TopAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), textureTop),
        _LegsAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), textureLegs), _MovementSwitch(false) {
    init();
}

//Mech::Mech(sf::Packet &packet, sf::Texture &textureTop, sf::Texture &textureLegs, sf::Texture &bulletTexture,
//           PlayerCar &player) :
//        BossCar(packet, textureTop, bulletTexture, player),
//        _TopAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), textureTop),
//        _LegsAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), textureLegs), _MovementSwitch(false) {
//    init();
//}

void Mech::render(sf::RenderWindow &window) {
    _LegsAnim.render(window);
    _TopAnim.render(window);

    window.draw(_HealthBar);
    window.draw(_HealthBarFrame);

//    renderExplosions(window);
}

void Mech::update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<Bullet>> &bullets) {
    Car::update(frameTime);

    float gunPosLength = std::sqrt(std::pow(_GunOffset.x, 2) + std::pow(_GunOffset.y, 2));
//    _GunOffset = sf::Vector2f(cosf(std::atan(_GunOrientation.y / _GunOrientation.x) + PI / 2),
//                                sinf(std::atan(_GunOrientation.y / _GunOrientation.x) + PI / 2)) * gunPosLength;

//    if (!_IsExploding) {
//        if (driveToNextPosition(frameTime)) {
//            switch (_MovementCommand) {
//                case Movement::DRIVETODEFAULT:
//                    _MovementCommand = Movement::LEFTRIGHT;
//                    _Attack = true;
//                    _PhaseClock.restart();
//                    _Speed = _BaseSpeed;
//                    break;
//                case Movement::LEFTRIGHT:
//                    _MovementSwitch = !_MovementSwitch;
//
//                    if (_MovementSwitch) {
//                        _NextPosition = getPos() + sf::Vector2f(
//                                (SCREENWIDTH - getPos().x - getWidth() / 2) * (std::rand() % 100) / 100.0f, 0.0f);
//                    } else {
//                        _NextPosition = getPos() -
//                                        sf::Vector2f((getPos().x - getWidth() / 2) * (std::rand() % 100) / 100.0f,
//                                                     0.0f);
//                    }
//                    break;
//                case Movement::RUNATPLAYER:
//                    _MovementCommand = Movement::DRIVETODEFAULT;
//                    _NextPosition = _DefaultPosition;
//                    break;
//                case Movement::ZICKZACK:
//                    if (_Event1Counter < 4) {
//                        _Event1Counter++;
//                        _NextPosition = sf::Vector2f(std::abs((int) _Event2Switch * SCREENWIDTH - 65) +
//                                                     std::pow(-1, (int) _Event2Switch) * (_Event1Counter % 2) *
//                                                     (SCREENWIDTH - 130), (SCREENHEIGHT / 5) * _Event1Counter);
//                    } else {
//                        _Event1Counter = 0;
//                        _MovementCommand = Movement::DRIVETODEFAULT;
//                    }
//                default:
//                    break;
//            }
//        }
//
//        if (_Attack) {
//            switch (_Pattern[_CurrentPhase].first) {
//                case Phase::SPIN: {
//                    _Event1Frequency = 11.0f + 3.0f * (float) _Difficulty;
//
//                    float gunAngle = rh::getAngleFromVector(_GunOrientation) + (180 + 135 * _Difficulty) * frameTime;
//                    _GunOrientation = sf::Vector2f(std::cos(gunAngle * PI / 180), std::sin(gunAngle * PI / 180));
//
//                    if (getBossEvent() == 1) {
//                        std::pair<sf::Vector2f, sf::Vector2f> Positions = calcGunPositions();
//
//                        BossCar::shootBullet(bullets, Positions.first, _GunOrientation);
//                        BossCar::shootBullet(bullets, Positions.second, _GunOrientation);
//                    }
//                    break;
//                }
//                case Phase::SHOTGUN:
//                    _GunOrientation = rh::normalize(player.getPosition() - getPos());
//
//                    _Event1Frequency = 1.0f + 0.25f * (float) (_Difficulty);
//
//                    if (getBossEvent() == 1) {
//                        bool Hand = (std::rand() % 100) > 50;
//                        for (int i = 0; i < 8 + 2 * _Difficulty; i++) {
//                            float bulAngle = (rh::getAngleFromVector(_GunOrientation) * PI / 180.f - PI / 28.8f) +
//                                             PI / 14.4f * ((std::rand() % 100) / 100.0f);
//                            sf::Vector2f bulOrientation = rh::normalize(
//                                    sf::Vector2f(std::cos(bulAngle), std::sin(bulAngle)));
//                            sf::Vector2f pos;
//                            if (Hand) {
//                                pos = calcGunPositions().first;
//                            } else {
//                                pos = calcGunPositions().second;
//                            }
//                            int speed = (int) (0.75 * _BulletSpeed +
//                                               ((std::rand() % 100) / 100.0f * 0.5 * _BulletSpeed));
//                            shootBullet(bullets, calcGunPositions().first, bulOrientation, speed);
//                        }
//
//                        _Event1Counter = 0;
//                        _Event1Switch = !_Event1Switch;
//                    }
//                    break;
//                case Phase::SALVE:
//                    _GunOrientation = rh::normalize(player.getPosition() - getPos());
//
//                    _Event1Frequency = 1.25f + 0.25f * (float) _Difficulty;
//
//                    if (getBossEvent() == 1) {
//                        bool Hand = _Event1Switch;//(std::rand() % 100) > 50;
//                        _Event1Switch = !_Event1Switch;
//
//                        float bulAngle = rh::getAngleFromVector(_GunOrientation);
//
//                        int NumberofBullets = (int) (5 + 0.5f * (float) _Difficulty);
//                        float dAngle = (42.5f + 2.5f * (float) _Difficulty) / (float) NumberofBullets;
//
//                        for (int i = 0; i < NumberofBullets; i++) {
//                            sf::Vector2f bulOrientation = sf::Vector2f(
//                                    std::cos((bulAngle + (i - NumberofBullets / 2) * dAngle) / 180 * PI),
//                                    std::sin((bulAngle + (i - NumberofBullets / 2) * dAngle) / 180 * PI));
//
//                            if (Hand) {
//                                BossCar::shootBullet(bullets, calcGunPositions().first, bulOrientation);
//                            } else {
//                                BossCar::shootBullet(bullets, calcGunPositions().second, bulOrientation);
//                            }
//                        }
//                    }
//                    break;
//                case Phase::SALVEZICKZACK:
//                    _GunOrientation = rh::normalize(player.getPosition() - getPos());
//
//                    _Event1Frequency = 1.25f + 0.25f * (float) _Difficulty;
//
//                    if (getBossEvent() == 1) {
//                        bool Hand = _Event1Switch;//(std::rand() % 100) > 50;
//                        _Event1Switch = !_Event1Switch;
//
//                        float bulAngle = rh::getAngleFromVector(_GunOrientation);
//
//                        int NumberofBullets = (int) (3 + 0.5f * (float) _Difficulty);
//                        float dAngle = (37.0f + 2.5f * (float) _Difficulty) / (float) NumberofBullets;
//
//                        for (int i = 0; i < NumberofBullets; i++) {
//                            sf::Vector2f bulOrientation = sf::Vector2f(
//                                    std::cos((bulAngle + (i - NumberofBullets / 2) * dAngle) / 180 * PI),
//                                    std::sin((bulAngle + (i - NumberofBullets / 2) * dAngle) / 180 * PI));
//
//                            if (Hand) {
//                                BossCar::shootBullet(bullets, calcGunPositions().first, bulOrientation);
//                            } else {
//                                BossCar::shootBullet(bullets, calcGunPositions().second, bulOrientation);
//                            }
//                        }
//                    }
//                    break;
//                case Phase::RUNATPLAYERPHASE:
//                    if (!_Event1Switch) {
//                        _NextPosition = player.getPosition();
//                        _MovementCommand = Movement::RUNATPLAYER;
//                        _Speed = (3.5 + 2 * _Difficulty) * _BaseSpeed;
//                        _Attack = false;
//
//                        _Event1Switch = true;
//                    }
//                    break;
//                case Phase::ZICKZACKPHASE:
//                    if (!_Event1Switch) {
//                        _NextPosition = _DefaultPosition;
//                        _MovementCommand = Movement::ZICKZACK;
//                        _CurrentPhase++;
//                        _Speed = (2 + _Difficulty) * _BaseSpeed;
//                        _Attack = true;
//                        _Event1Switch = true;
//                        _Event2Switch = (std::rand() % 100) > 50;
//                    }
//                case NOTHING:
//                    break;
//                case SIMPLESHOOT:
//                    break;
//                case HARDCORESPAM:
//                    break;
//                case BLASTSALVE:
//                    break;
//                case SPIRAL:
//                    break;
//                case RANDOMSPRAY:
//                    break;
//                case SIDE:
//                    break;
//                case SAVELANES:
//                    break;
//            }
//        }
//
//        _TopAnim.setRotation(rh::getAngleFromVector(_GunOrientation) + 90);
//        _LegsAnim.setRotation(rh::getAngleFromVector(_GunOrientation) + 90);
//
//        updateHealthBar();
//
//        if (_MovementCommand != Movement::DRIVETODEFAULT && _MovementCommand != Movement::ZICKZACK) {
//            checkPhase();
//        }
//
//        _LegsAnim.update(frameTime);
//        _TopAnim.update(frameTime);
//    } else {
//        updateExplosions(frameTime);
//    }
}

void Mech::setPos(sf::Vector2f pos) {
    _TopAnim.getSprite().setPosition(pos);
    _LegsAnim.getSprite().setPosition(pos);
}

void Mech::init() {
    setSprite(_TopAnim.getSprite());

//    _GunOrientation = sf::Vector2f(0, -1);
    _GunOffset = sf::Vector2f(62, 0);
//    _GunLength = 53.0f;

    //HP-Balken
    _HealthBar.setSize(sf::Vector2f(getWidth() + 5, 5));
    _HealthBarFrame.setSize(_HealthBar.getSize());

    _DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT - 200);
    _NextPosition = _DefaultPosition;

    _BaseSpeed = _Speed;

//    _MovementCommand = Movement::DRIVETODEFAULT;
//    _Pattern = {
//            std::make_pair(Phase::SPIN,
//                           ((2.0f + (float) _Difficulty) * 360.0f) / (180.0f + 135.f * (float) _Difficulty)),
//            std::make_pair(Phase::NOTHING, 1.5f),
//            std::make_pair(Phase::SALVE, 7.0f), std::make_pair(Phase::NOTHING, 0.75f),
//            std::make_pair(Phase::RUNATPLAYERPHASE, 0.25f),
//            std::make_pair(Phase::SHOTGUN, 7.0f), std::make_pair(Phase::NOTHING, 1.5f),
//            std::make_pair(Phase::ZICKZACKPHASE, 0.25f), std::make_pair(Phase::SALVEZICKZACK, 3.0f),
//            std::make_pair(Phase::NOTHING, 0.75f)
//    };
}

std::pair<sf::Vector2f, sf::Vector2f> Mech::calcGunPositions() {
//    sf::Vector2f pos1 = getPos() - _GunOffset + _GunOrientation * _GunLength;
//    sf::Vector2f pos2 = getPos() + _GunOffset + _GunOrientation * _GunLength;
//    return std::make_pair(pos1, pos2);
}
