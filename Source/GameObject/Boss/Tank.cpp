#include "stdafx.h"
#include <GameObject/Boss/Action/MoveToPosition.h>
#include <GameObject/Boss/Action/MoveLeftRight.h>
#include "GameObject/Boss/Tank.h"
#include "GameObject/Boss/Action/ShootAtPlayer.h"
#include "GameObject/Boss/Action/TimerAction.h"


Tank::Tank(unsigned int id, GameObjectManager &gom, int difficulty, int HP, sf::Texture &texture,
           sf::Texture &bulletTexture) : BossCar(id, gom,
                                                 sf::Vector2f(SCREENWIDTH / 2, -1 * (float) texture.getSize().y / 2.0f),
                                                 difficulty, HP, 200, texture, bulletTexture),
                                         _MovementSwitch(false) {
    init();
}

//Tank::Tank(sf::Packet &packet, sf::Texture &texture, sf::Texture &bulletTexture, PlayerCar &player) :
//        BossCar(packet, texture, bulletTexture, player),
//        _MovementSwitch(false) {
//    init();
//}

void Tank::render(sf::RenderWindow &window) {
    window.draw(getSprite());
    window.draw(_GunSprite);

    window.draw(_HealthBar);
    window.draw(_HealthBarFrame);

    renderExplosions(window);
}

void Tank::update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<Bullet>> &bullets) {


    BossCar::update(frameTime);

    _GunSprite.setPosition(getPosition());
}

//    if (!_IsExploding) {
//        if (driveToNextPosition(frameTime)) {
//            switch (_MovementCommand) {
//                case Movement::DRIVETODEFAULT:
//				{
//					_MovementCommand = Movement::LEFTRIGHT;
//					_Speed = 200;
//					_Attack = true;
//					_PhaseClock.restart();
//
//				} break;
//
//				case Movement::LEFTRIGHT:
//				{
//					_MovementSwitch = !_MovementSwitch;
//					if (_MovementSwitch)
//					{
//						_NextPosition = getPosition() + sf::Vector2f(
//							(SCREENWIDTH - getPosition().x - getWidth() / 2) * (std::rand() % 100) / 100.0f, 0.0f);
//					}
//					else
//					{
//						_NextPosition = getPosition() -
//							sf::Vector2f((getPosition().x - getWidth() / 2) * (std::rand() % 100) / 100.0f,
//								0.0f);
//					}
//				} break;
//
//                default:
//                    break;
//            }
//        }
//
//        if (_Attack) {
//            float angle = rh::getAngleFromVector(_GunOrientation);
//
//            switch (_Pattern[_CurrentPhase].first) {
//                case Phase::SIMPLESHOOT:
//                    _Event1Frequency = 4.25f + 1.25f * (float) _Difficulty;
//
//                    _GunOrientation = rh::normalize(player.getPosition() - getPosition());
//
//                    if (getBossEvent() == 1) {
//                        BossCar::shootBullet(bullets, calcBulletPosition(), _GunOrientation);
//                    }
//                    break;
//                case Phase::SALVE:
//
//                    _Event1Frequency = 1.0f + 0.75f * (float) _Difficulty;
//                    _Event2Frequency = 10.0f + 1.0f * (float) _Difficulty;
//
//                    _GunOrientation = rh::normalize(player.getPosition() - getPosition());
//
//                    if (_Event1Switch) {
//                        if (getBossEvent() == 2) {
//                            BossCar::shootBullet(bullets, calcBulletPosition(), _GunOrientation);
//                            if (_Event1Counter + 1 < 3) {
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
//                case Phase::SPIN:
//                    _Event1Frequency = 9.0f + 1.0f * (float) _Difficulty;
//                    if (_Event1Switch) {
//                        if (angle + frameTime * 100 > 180.0f) {
//                            _Event1Switch = false;
//                        } else {
//                            angle += frameTime * 100;
//                        }
//                    } else {
//                        if (angle - frameTime * 100 < 0.0f) {
//                            _Event1Switch = true;
//                        } else {
//                            angle -= frameTime * 100;
//                        }
//                    }
//                    _GunOrientation = rh::normalize(
//                            sf::Vector2f(std::cos(angle * PI / 180), std::sin(angle * PI / 180)));
//
//                    if (getBossEvent() == 1) {
//                        BossCar::shootBullet(bullets, calcBulletPosition(), _GunOrientation);
//                    }
//                    break;
//                case Phase::HARDCORESPAM:
//                    _Event1Frequency = 40.0f + 15.0f * (float) _Difficulty;
//                    if (getBossEvent() == 1) {
//                        _Event1Counter++;
//                        _GunOrientation = rh::normalize(
//                                sf::Vector2f(((std::rand() - (float) (RAND_MAX) / 2) / (float) (RAND_MAX)),
//                                             ((std::rand() - (float) (RAND_MAX) / 2) / (float) (RAND_MAX))));
//
//						BossCar::shootBullet(bullets, calcBulletPosition(), _GunOrientation);
//                    }
//                    break;
//                case NOTHING:
//                    break;
//                case BLASTSALVE:
//                    break;
//                case SPIRAL:
//                    break;
//                case RANDOMSPRAY:
//                    break;
//                case SHOTGUN:
//                    break;
//                case SIDE:
//                    break;
//                case SAVELANES:
//                    break;
//                case RUNATPLAYERPHASE:
//                    break;
//                case ZICKZACKPHASE:
//                    break;
//                case SALVEZICKZACK:
//                    break;
//            }
//
//        }
//        _GunSprite.setPosition(getPosition() + _GunPosition);
//        _GunSprite.setRotation(rh::getAngleFromVector(_GunOrientation) - 90);
//
//        updateHealthBar();
//        checkPhase();
//    } else {
//        updateExplosions(frameTime);
//    }
//}

void Tank::init() {
    _GunTexture.loadFromFile("Resources/Texture/BossCar/CannonTank.png");
    _GunSprite.setTexture(_GunTexture);
    _GunSprite.setOrigin(_GunTexture.getSize().x / 2, 50);

    _GunPosition = sf::Vector2f(0, -15);
    _GunOrientation = sf::Vector2f(0, 1);
    _GunLength = _GunSprite.getLocalBounds().height - 50;

    _DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, 150);
//    _NextPosition = _DefaultPosition;

    _NextPhase = 0;
    updateActions();

//    _MovementCommand = Movement::DRIVETODEFAULT;
//    _Pattern = {std::make_pair(Phase::SIMPLESHOOT, 4.0f), std::make_pair(Phase::NOTHING, 0.75f),
//                std::make_pair(Phase::SPIN, 10.0f), std::make_pair(Phase::NOTHING, 0.75f),
//                std::make_pair(Phase::SALVE, 10.0f), std::make_pair(Phase::NOTHING, 0.75f),
//                std::make_pair(Phase::HARDCORESPAM, 6.0f), std::make_pair(Phase::NOTHING, 0.75f)};
}

void Tank::updateActions() {
    std::cout << "Phase: " << _NextPhase << std::endl;
    _Actions = std::vector<std::shared_ptr<BossAction>>();
    switch (_NextPhase) {
        case 1: { // Move left and right and shoot at player
            sf::Vector2f leftPosition = sf::Vector2f(getWidth() / 2 + 20, 150);
            sf::Vector2f rightPosition = sf::Vector2f(SCREENWIDTH - (getWidth() / 2 + 20), 150);
            std::shared_ptr<MoveLeftRight> moveAction = std::make_shared<MoveLeftRight>(*this, leftPosition,
                                                                                        rightPosition, 1);
            std::vector<float> shotFrequencies = {0.5f, 0.05f, 0.05f};
            std::vector<float> salveAngles = {0};
            std::shared_ptr<ShootAtPlayer> shootAction = std::make_shared<ShootAtPlayer>(*this, moveAction,
                                                                                         shotFrequencies,
                                                                                         salveAngles);

//            std::shared_ptr<ShootAtPlayer> shootAction = std::make_shared<ShootAtPlayer>(*this, nullptr,
//                                                                                         shotFrequencies,
//                                                                                         salveAngles);
            _Actions.push_back(moveAction);
            _Actions.push_back(shootAction);
            break;
        }
        case 2: {
            std::shared_ptr<MoveToPosition> moveAction = std::make_shared<MoveToPosition>(*this, _DefaultPosition);
            std::shared_ptr<TimerAction> timerAction = std::make_shared<TimerAction>(*this, 10.0f);
            std::vector<float> shotFrequencies = {0.5f, 0.05f, 0.05f};
            std::vector<float> salveAngles = {-PI / 8, -PI / 16, 0, PI / 16, PI / 8};
            std::shared_ptr<ShootAtPlayer> shootAction = std::make_shared<ShootAtPlayer>(*this, timerAction,
                                                                                         shotFrequencies,
                                                                                         salveAngles);
            _Actions.push_back(moveAction);
            _Actions.push_back(timerAction);
            _Actions.push_back(shootAction);
            break;
        }
        default: {
            std::shared_ptr<MoveToPosition> action = std::make_shared<MoveToPosition>(*this, _DefaultPosition);
            _Actions.push_back(action);
            _NextPhase = 0;
            break;
        }
    }
    _NextPhase++;
}
