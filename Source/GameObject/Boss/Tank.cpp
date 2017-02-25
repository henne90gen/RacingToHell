#include "stdafx.h"
#include "GameObject/Boss/Tank.h"


Tank::Tank(unsigned int id, int difficulty, int HP, sf::Texture &texture, sf::Texture &bulletTexture) :
        BossCar(id, sf::Vector2f(SCREENWIDTH / 2, -1 * (float) texture.getSize().y / 2.0f), difficulty, HP, 200,
                texture, bulletTexture),
        _MovementSwitch(false) {
    init();
}

Tank::Tank(sf::Packet &packet, sf::Texture &texture, sf::Texture &bulletTexture) :
        BossCar(packet, texture, bulletTexture),
        _MovementSwitch(false) {
    init();
}

void Tank::render(sf::RenderWindow &window) {
    window.draw(getSprite());
    window.draw(_GunSprite);

    window.draw(_HealthBar);
    window.draw(_HealthBarFrame);

    renderExplosions(window);
}

void Tank::update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<Bullet>> &bullets, PlayerCar &player) {
    Car::update(frameTime, roadSpeed);

    if (!_IsExploding) {
        if (driveToNextPosition(frameTime)) {
            switch (_Movement) {
                case Movement::DRIVETODEFAULT:
				{
					_Movement = Movement::LEFTRIGHT;
					_Speed = 200;
					_Attack = true;
					_PhaseClock.restart();

				} break;
                
				case Movement::LEFTRIGHT:
				{  
					_MovementSwitch = !_MovementSwitch;
					if (_MovementSwitch) 
					{
						_NextPosition = getPos() + sf::Vector2f(
							(SCREENWIDTH - getPos().x - getWidth() / 2) * (std::rand() % 100) / 100.0f, 0.0f);
					}
					else 
					{
						_NextPosition = getPos() -
							sf::Vector2f((getPos().x - getWidth() / 2) * (std::rand() % 100) / 100.0f,
								0.0f);
					}
				} break;

                default:
                    break;
            }
        }

        if (_Attack) {
            float angle = rh::getAngleFromVector(_GunOrientation);

            switch (_Pattern[_CurrentPhase].first) {
                case Phase::SIMPLESHOOT:
                    _Event1Frequency = 4.25f + 1.25f * (float) _Difficulty;

                    _GunOrientation = rh::normalize(player.getPos() - getPos());

                    if (getBossEvent() == 1) {
                        BossCar::shootBullet(bullets, calcBulletPosition(), _GunOrientation);
                    }
                    break;
                case Phase::SALVE:

                    _Event1Frequency = 1.0f + 0.75f * (float) _Difficulty;
                    _Event2Frequency = 10.0f + 1.0f * (float) _Difficulty;

                    _GunOrientation = rh::normalize(player.getPos() - getPos());

                    if (_Event1Switch) {
                        if (getBossEvent() == 2) {
                            BossCar::shootBullet(bullets, calcBulletPosition(), _GunOrientation);
                            if (_Event1Counter + 1 < 3) {
                                _Event1Counter += 1;
                            } else {
                                _Event1Switch = false;
                                _Event1Counter = 0;
                            }
                        }
                    } else {
                        if (getBossEvent() == 1) {
                            _Event1Switch = true;
                        }
                    }
                    break;
                case Phase::SPIN:
                    _Event1Frequency = 9.0f + 1.0f * (float) _Difficulty;
                    if (_Event1Switch) {
                        if (angle + frameTime * 100 > 180.0f) {
                            _Event1Switch = false;
                        } else {
                            angle += frameTime * 100;
                        }
                    } else {
                        if (angle - frameTime * 100 < 0.0f) {
                            _Event1Switch = true;
                        } else {
                            angle -= frameTime * 100;
                        }
                    }
                    _GunOrientation = rh::normalize(
                            sf::Vector2f(std::cos(angle * PI / 180), std::sin(angle * PI / 180)));

                    if (getBossEvent() == 1) {
                        BossCar::shootBullet(bullets, calcBulletPosition(), _GunOrientation);
                    }
                    break;
                case Phase::HARDCORESPAM:
                    _Event1Frequency = 40.0f + 15.0f * (float) _Difficulty;
                    if (getBossEvent() == 1) {
                        _Event1Counter++;
                        _GunOrientation = rh::normalize(
                                sf::Vector2f(((std::rand() - (float) (RAND_MAX) / 2) / (float) (RAND_MAX)),
                                             ((std::rand() - (float) (RAND_MAX) / 2) / (float) (RAND_MAX))));

						BossCar::shootBullet(bullets, calcBulletPosition(), _GunOrientation);
                    }
                    break;
                case NOTHING:
                    break;
                case BLASTSALVE:
                    break;
                case SPIRAL:
                    break;
                case RANDOMSPRAY:
                    break;
                case SHOTGUN:
                    break;
                case SIDE:
                    break;
                case SAVELANES:
                    break;
                case RUNATPLAYERPHASE:
                    break;
                case ZICKZACKPHASE:
                    break;
                case SALVEZICKZACK:
                    break;
            }

        }
        _GunSprite.setPosition(getPos() + _GunPosition);
        _GunSprite.setRotation(rh::getAngleFromVector(_GunOrientation) - 90);

        updateHealthBar();
        checkPhase();
    } else {
        updateExplosions(frameTime);
    }
}

void
Tank::shootBullet(std::vector<std::shared_ptr<Bullet>> &bullets, sf::Vector2f pos, sf::Vector2f dir, int bulletSpeed) {
    bullets.push_back(
            GameObjectFactory::getBullet(pos, dir, bulletSpeed, GameObjectType::BulletObjectBoss));
}

void Tank::init() {
    _GunTexture.loadFromFile("Resources/Texture/BossCar/CannonTank.png");
    _GunSprite.setTexture(_GunTexture);
    _GunSprite.setOrigin(_GunTexture.getSize().x / 2, 50);

    _GunPosition = sf::Vector2f(0, -15);
    _GunOrientation = sf::Vector2f(0, 1);
    _GunLength = _GunSprite.getLocalBounds().height - 50;

    _DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, 150);
    _NextPosition = _DefaultPosition;
    _Movement = Movement::DRIVETODEFAULT;

    _Pattern = {std::make_pair(Phase::SIMPLESHOOT, 4.0f), std::make_pair(Phase::NOTHING, 0.75f),
                std::make_pair(Phase::SPIN, 10.0f), std::make_pair(Phase::NOTHING, 0.75f),
                std::make_pair(Phase::SALVE, 10.0f), std::make_pair(Phase::NOTHING, 0.75f),
                std::make_pair(Phase::HARDCORESPAM, 6.0f), std::make_pair(Phase::NOTHING, 0.75f)};
}
