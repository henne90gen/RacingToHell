#include "stdafx.h"
#include "GameObject/Car.h"

Car::Car(unsigned int id, sf::Vector2f pos, int maxhealth, float speed, GameObjectType Type, sf::Texture &texture,
         sf::IntRect textureRect) :
        GameObject(id, pos, Type, texture, textureRect) {
    _Speed = speed;
    _MaxHealth = maxhealth;
    _Health = maxhealth;
    _resetColorTime = -1.0f;

    _ImpactSoundBuffer.loadFromFile("Resources/Sound/impact.wav");
}

int Car::getHealth() {
    if (_Health < 0) {
        _Health = 0;
    }
    return _Health;
}

void Car::takeDamage(int damage) {
    _Health -= damage;
    getSprite().setColor(sf::Color(220, 0, 0));
    _resetColorTime = 0.0f;
}

void Car::operator>>(sf::Packet &packet) {
    GameObject::operator>>(packet);
    write(packet, _Speed);
    write(packet, _Health);
    write(packet, _MaxHealth);
}

void Car::operator<<(sf::Packet &packet) {
    GameObject::operator<<(packet);
    read(packet, _Speed);
    read(packet, _Health);
    read(packet, _MaxHealth);
}

void Car::update(float frametime, int roadspeed) {
    if (_resetColorTime >= 0) {
        _resetColorTime += frametime;

        if (_resetColorTime > 0.055f) {
            getSprite().setColor(sf::Color::White);
            _resetColorTime = -1.0f;
        }
    }
}
