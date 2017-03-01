#include "stdafx.h"
#include "GameObject/PlayerCar.h"


PlayerCar::PlayerCar(unsigned int id, GameObjectManager &gom, PlayerCarIndex selectedCar, sf::Texture &texture,
                     sf::Texture &explosionTexture) :
        Car(id, gom, sf::Vector2f(0, 0), 100, 500, GameObjectType::Player, texture,
            sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y)),
        _CrosshairSpeed(600.0f), _PlayerCarIndex(selectedCar), _AccelerationTime(0.1f),
        _ExplosionTexture(explosionTexture) {

    setStats(_PlayerCarIndex);
    _ShotBullet = sf::Vector2f(0, 0);
    setPos(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT - 300));

    _AimLine.setFillColor(sf::Color::Black);
    _AimLine.setSize(sf::Vector2f(50.0f, 3.0f));
    _AimLine.setOrigin(0, _AimLine.getSize().y / 2.0f);

    if (_CrosshairTexture.loadFromFile("Resources/Texture/PlayerCar/crosshair.png")) {
        _Crosshair.setTexture(_CrosshairTexture);
        _Crosshair.setOrigin(_Crosshair.getLocalBounds().width / 2.0f, _Crosshair.getLocalBounds().height / 2.0f);
        _Crosshair.setScale(sf::Vector2f(0.15f, 0.15f));
        _Crosshair.setPosition(SCREENWIDTH / 2.0f, SCREENHEIGHT / 2.0f);
    }

    sf::Listener::setDirection(0.f, 0.f, -1.f);
}

//PlayerCar::PlayerCar(sf::Packet &packet, std::vector<std::shared_ptr<sf::Texture>> &textures,
//                     sf::Texture &explosionTexture) :
//        Car(packet, GameObjectType::Player), _ExplosionTexture(explosionTexture) {
//    PlayerCar::operator<<(packet);
//    setStats(_PlayerCarIndex);
//    initTexture(*textures.at((int) _PlayerCarIndex), sf::IntRect());
//}

void PlayerCar::render(sf::RenderWindow &window, bool renderCrosshair) {
    if (isAlive() && !isDying()) {
        window.draw(getSprite());
        if (renderCrosshair) {
            window.draw(_AimLine);
            window.draw(_Crosshair);
        }
    } else {
        _Animation->render(window);
    }
}


void PlayerCar::applyKeyPress(sf::Uint8 keys) {
    bool Up, Right, Down, Left;
    Up = keys & (sf::Uint8) Key::Up;
    Right = keys & (sf::Uint8) Key::Right;
    Down = keys & (sf::Uint8) Key::Down;
    Left = keys & (sf::Uint8) Key::Left;

    if (Left) {
        _Movement.x = _Speed * -1.0f;
    } else if (Right) {
        _Movement.x = _Speed;
    } else {
        _Movement.x = 0.0f;
    }

    if (Up) {
        _Movement.y = _Speed * (4.0f/5.0f) * -1.0f;
    } else if (Down) {
        _Movement.y = _Speed * (6.0f/5.0f);
    } else {
        _Movement.y = 0.0f;
    }
}

void PlayerCar::handleEvent(sf::Event &Event) {
    // Apply key input to car
    _PressedKeys = 0;
    _PressedKeys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) *
                    (sf::Uint8) Key::Up;
    _PressedKeys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) *
                    (sf::Uint8) Key::Right;
    _PressedKeys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) *
                    (sf::Uint8) Key::Down;
    _PressedKeys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) *
                    (sf::Uint8) Key::Left;

    // Apply mouse movement
    if (Event.type == sf::Event::MouseMoved) {
        _Crosshair.setPosition(Event.mouseMove.x, Event.mouseMove.y);
    } else if (Event.type == sf::Event::MouseButtonPressed ||
               (Event.type == sf::Event::JoystickButtonPressed && sf::Joystick::isButtonPressed(0, 5))) {
        shoot();
        _AutoFireTimer.restart();
    }
}

void PlayerCar::update(float frameTime) {

    if (isDying()) {
        _Animation->update(frameTime);
    }

    // TODO re-enable auto fire
    /*std::cout << "Time: " << _AutoFireTimer.getElapsedTime().asSeconds() << std::endl;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (_AutoFireTimer.getElapsedTime().asSeconds() >= 0.175f) {
            shoot();
            _AutoFireTimer.restart();
        }
    }*/

    // Set the position of the sound listener
    sf::Listener::setPosition(getPosition().x, 0.f, getPosition().y);

    // Update the position according to movement
    Car::update(frameTime);

    if (getPosition().x < getWidth() / 2.0f) {
        setPos(sf::Vector2f(getWidth() / 2.0f, getPosition().y));
    } else if (getPosition().x > SCREENWIDTH - getWidth() / 2.0f) {
        setPos(sf::Vector2f(SCREENWIDTH - getWidth() / 2.0f, getPosition().y));
    }

    if (getPosition().y < getHeight() / 2.0f) {
        setPos(sf::Vector2f(getPosition().x, getHeight() / 2.0f));
    } else if (getPosition().y > SCREENHEIGHT - getHeight() / 2.0f) {
        setPos(sf::Vector2f(getPosition().x, SCREENHEIGHT - getHeight() / 2.0f));
    }

    // TODO replace aimline with something cooler
    // Update _AimLine
    _AimLine.setPosition(getPosition());
    sf::Vector2f dir = _Crosshair.getPosition() - getPosition();
    float angle = std::atan(dir.y / dir.x) * 180.0f / PI;
    if (dir.x < 0) {
        _AimLine.setRotation(angle + 180);
    } else {
        _AimLine.setRotation(angle);
    }
}

bool PlayerCar::drainShotEnergy() {
    if (_Energy - 5 >= 10) {
        _Energy -= 5;
        return true;
    }
    return false;
}

void PlayerCar::shoot() {
    _ShotBullet = _Crosshair.getPosition() - getPosition();
    _ShotBullet = rh::normalize(_ShotBullet);
}

sf::Vector2f PlayerCar::getShotBullet() {
    sf::Vector2f tmp = _ShotBullet;
    _ShotBullet = sf::Vector2f(0, 0);
    return tmp;
}

void PlayerCar::addHealth() {
    if (_Health + 20 > _MaxHealth) {
        _Health = _MaxHealth;
    } else {
        _Health += 20;
    }
}

void PlayerCar::addEnergy() {
    if (_Energy + 50 > _MaxEnergy) {
        _Energy = _MaxEnergy;
    } else {
        _Energy += 50;
    }
}

void PlayerCar::setStats(PlayerCarIndex id) {
    _PlayerCarIndex = id;
    std::vector<int> Stats = PlayerStats::getPlayerStats(id);
    _MaxHealth = (sf::Int16) Stats[0];
    _Health = _MaxHealth;
    _MaxEnergy = (sf::Uint16) Stats[1];
    _Energy = _MaxEnergy;
    _Speed = (sf::Int16) Stats[2];
    _Bulletdamage = (sf::Uint16) Stats[3];
}

void PlayerCar::operator>>(sf::Packet &packet) {
    /*
     * _Type
     * _ID
     * _Pos.x
     * _Pos.y
     * _Speed
     * _Health
     * _MaxHealth
     * _PlayerCarIndex
     */

    Car::operator>>(packet);
    write(packet, (int) _PlayerCarIndex);
//    write(packet, _Force.x);
//    write(packet, _Force.y);
    write(packet, _Movement.x);
    write(packet, _Movement.y);
}

void PlayerCar::operator<<(sf::Packet &packet) {
    //Car::operator<<(packet);
    read(packet, _ID);
    float x, y;
    read(packet, x);
    read(packet, y);


    read(packet, _Speed);
    read(packet, _Health);
    read(packet, _MaxHealth);
    int selectedCar = 0;
    read(packet, selectedCar);
    _PlayerCarIndex = (PlayerCarIndex) selectedCar;
    setStats(_PlayerCarIndex);
    //std::cout << "Updated position" << _ID << ": " << getPos().x << " " << getPosition().y << std::endl;
//    read(packet, _Force.x);
//    read(packet, _Force.y);
    float vx, vy;
    read(packet, vx);
    read(packet, vy);

    //std::cout << "dX = " << std::abs(getPos().x - x) << " dY = " << std::abs(getPosition().y - y) << " dVx = " << std::abs(_Movement.x - vx) << std::endl;
    if (std::abs(getPosition().x - x) > 1 || std::abs(getPosition().y - y) > 1) {
        setPos(sf::Vector2f(x, y));
    }

    _Movement = sf::Vector2f(vx, vy);
}

bool PlayerCar::isAlive() {
    return _Energy > 0 && _Health > 0;
}

void PlayerCar::kill() {
    _Health = 0;
    _Movement = sf::Vector2f(0, 0);
    _Animation = std::make_shared<Explosion>(getPosition(), _ExplosionTexture, sf::Vector2f(0, 0));
    _Animation->play();
}

void PlayerCar::takeDamage(int damage) {
    Car::takeDamage(damage);
    if (_Health <= 0 && !isDying()) {
        kill();
    }
}

void PlayerCar::drainEnergy(float frameTime) {
    _Energy -= 2 * frameTime;
    if (_Energy <= 0 && !isDying()) {
        kill();
    }
}

bool PlayerCar::isDying() {
    return _Health <= 0 && _Animation->getAnimationState() == Animation::Play;
}
