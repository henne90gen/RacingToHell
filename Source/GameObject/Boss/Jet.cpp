#include "stdafx.h"
#include "GameObject/Boss/Jet.h"

Jet::Jet(unsigned int id, GameObjectManager &gom, int difficulty, int HP, sf::Texture &texture,
         sf::Texture &bulletTexture,
         sf::SoundBuffer &soundBufferEngine) :
        BossCar(id, gom, sf::Vector2f(-1 * texture.getSize().x, SCREENHEIGHT / 2), difficulty, HP, 500, texture,
                bulletTexture),
        _EngineSoundBuffer(soundBufferEngine) {
    init();
}

//Jet::Jet(sf::Packet &packet, sf::Texture &texture, sf::Texture &bulletTexture, sf::SoundBuffer &soundBufferEngine, PlayerCar &player) :
//        BossCar(packet, texture, bulletTexture, player) {
//    init();
//}

void Jet::render(sf::RenderWindow &window) {
    window.draw(getSprite());

    window.draw(_HealthBar);
    window.draw(_HealthBarFrame);

    renderExplosions(window);
}

void Jet::update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<Bullet>> &bullets) {
    Car::update(frameTime);

//    if (!_IsExploding) {
//        if (_MovementCommand != Movement::STILL) {
//            if (driveToNextPosition(frameTime)) {
//                //_BossEventTimer1.restart();
//                //_BossEventTimer2.restart();
//                //_Event1Switch = false;
//                //_Event2Switch = false;
//                //_Event2Counter = 0;
//                //_Event1Counter = 0;
//
//                _MovementCommand = Movement::STILL;
//            } else {
//                _EngineSound.setPosition(getPosition().x, 0.f, getPosition().y);
//                if (_EngineSound.getStatus() == sf::Sound::Paused || _EngineSound.getStatus() == sf::Sound::Stopped) {
//                    _EngineSound.play();
//                }
//            }
//        } else if (_MovementCommand == Movement::STILL) {
//            if (_EngineSound.getStatus() == sf::Sound::Playing) _EngineSound.stop();
//
//            switch (_Pattern[_CurrentPhase].first) {
//                case Phase::SIDE: {
//                    _Event1Frequency = 0.2f;
//
//                    if (getBossEvent() == 1) {
//                        for (int i = -40; i <= SCREENHEIGHT; i += 200) {
//                            BossCar::shootBullet(bullets, sf::Vector2f(0, i), sf::Vector2f(1, 0));
//                            BossCar::shootBullet(bullets, sf::Vector2f(SCREENWIDTH, i + 100), sf::Vector2f(-1, 0));
//                        }
//                    }
//                    break;
//                }
//                case Phase::SAVELANES: {
//                    _Event1Frequency = 0.2f;
//                    _Event2Frequency = 0.0f;
//
//                    if (getBossEvent() == 1 || _Event2Counter > 0) {
//                        _Event2Frequency = 3.0f + 1.0f * (float) _Difficulty;
//
//                        if (getBossEvent() == 2) {
//                            if (_Event2Counter + 1 <= 3 + 2 * _Difficulty) {
//                                for (int i = 0; i < 3; i++) {
//                                    BossCar::shootBullet(bullets, sf::Vector2f(i * 150 + 150, 0), sf::Vector2f(0, 1));
//                                }
//
//                                BossCar::shootBullet(bullets, sf::Vector2f(20, 0), sf::Vector2f(0, 1));
//                                BossCar::shootBullet(bullets, sf::Vector2f(SCREENWIDTH - 20, 0), sf::Vector2f(0, 1));
//
//                                ++_Event2Counter;
//                            } else {
//                                _Event2Counter = 0;
//                                _Event2Frequency = 0.0f;
//                            }
//                        }
//                    }
//                }
//                default:
//                    break;
//            }
//        }
//
//        if (_MovementCommand == Movement::PARABOLA) {
//            setPos(getPosition() + sf::Vector2f(0, (float) roadSpeed * frameTime));
//        }
//
//        checkPhase();
//        updateHealthBar();
//    } else {
//        updateExplosions(frameTime);
//    }
}

void Jet::randomPosition() {
    bool LeftRight = (std::rand() % 100) > 50;

    setPos(sf::Vector2f(
            -0.5f * getWidth() + std::pow(-1, (int) !LeftRight) * (SCREENWIDTH * (int) LeftRight + getWidth() * 4),
            0.5f * getHeight() + ((std::rand() % 100) / 100.0f) * (SCREENHEIGHT - getHeight())));
    getSprite().setRotation(-180.0f * (int) (LeftRight));

    _NextPosition = sf::Vector2f(std::pow(-1, (int) LeftRight) * (SCREENWIDTH * (int) !LeftRight + getWidth() * 4),
                                 getPosition().y);
}

void Jet::checkPhase() {
//    if (_PhaseClock.getElapsedTime().asSeconds() >= _Pattern[_CurrentPhase].second) {
//        if (_CurrentPhase + 1 >= _Pattern.size()) {
//            _CurrentPhase = 0;
//        } else {
//            ++_CurrentPhase;
//        }
//
//        _MovementCommand = Movement::STRAIGHT;
//        randomPosition();
//        _PhaseClock.restart();
//    }
}

void Jet::stopSounds() {
    _EngineSound.stop();
}

void Jet::init() {
    // FIXME move engine sound somewhere else
    _EngineSound.setBuffer(_EngineSoundBuffer);
//    _EngineSound.setVolume(_Volume * 5.5f);
    _EngineSound.setPosition(getPosition().x, 0.f, getPosition().y);
    _EngineSound.setMinDistance(600.f);
    _EngineSound.setAttenuation(8.f);

    _HasTraffic = true;
    _Speed = (0.8f + 0.2f * (float) _Difficulty) * _Speed;

    randomPosition();

//    _Pattern = {std::make_pair(Phase::SIDE, 6.0f)};
//    _CurrentPhase = 0;
//    _MovementCommand = Movement::STRAIGHT;
//    _Pattern = {std::make_pair(Phase::SIDE, 10.5f), std::make_pair(Phase::SAVELANES, 9.f)};
}
