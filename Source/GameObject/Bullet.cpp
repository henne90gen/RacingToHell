#include "stdafx.h"
#include "GameObject/Bullet.h"

Bullet::Bullet(unsigned int id, sf::Vector2f pos, sf::Vector2f dir, float speed, GameObjectType type,
               sf::Texture &texture) :
        GameObject(id, pos, type, texture, sf::IntRect(0, 0, 10, 10)),
        _Direction(dir), _Speed(speed) {

    if (type == GameObjectType::BulletObjectPlayer) {
        setSpriteColor(sf::Color(225, 0, 0));
//        playShotSound(pos, soundEffects, soundBuffer, Volume * 4.f);
    } else if (type == GameObjectType::BulletObjectBoss) {
        setSpriteColor(sf::Color(0, 45, 255));
//        playShotSound(pos, soundEffects, soundBuffer, Volume * 4.f);
    } else {
        setSpriteColor(sf::Color(255, 255, 0));
//        playShotSound(pos, soundEffects, soundBuffer, Volume * 5.5f);
    }
}

Bullet::Bullet(sf::Packet &packet, GameObjectType type, sf::Texture &texture) :
        GameObject(packet, type, texture, sf::IntRect(0, 0, 10, 10)) {
    Bullet::operator<<(packet);
    if (type == GameObjectType::BulletObjectPlayer) {
        setSpriteColor(sf::Color(225, 0, 0));
    } else if (type == GameObjectType::BulletObjectBoss) {
        setSpriteColor(sf::Color(0, 45, 255));
    } else {
        setSpriteColor(sf::Color(255, 255, 0));
    }
//    playShotSound(getPos(), soundEffects, soundBuffer, Volume);
}

Bullet::Bullet(sf::Packet &packet, sf::Vector2f PlayerPosition, GameObjectType type, sf::Texture &texture,
               sf::Uint32 ID, float Speed) {
    _Speed = Speed;
    _ID = ID;
    _Type = type;
    initTexture(texture, sf::IntRect(0, 0, 10, 10));
    setPos(PlayerPosition);

    float dx, dy;
    packet >> dx >> dy;

    _Direction = sf::Vector2f(dx, dy) - getPos();
    _Direction /= float(std::sqrt(std::pow(_Direction.x, 2) + std::pow(_Direction.y, 2)));

    if (type == GameObjectType::BulletObjectPlayer) {
        setSpriteColor(sf::Color(225, 0, 0));
    } else if (type == GameObjectType::BulletObjectBoss) {
        setSpriteColor(sf::Color(0, 45, 255));
    } else {
        setSpriteColor(sf::Color(255, 255, 0));
    }
}

Bullet::Bullet(GameObjectType type, sf::Texture &texture, float speed) : GameObject(0, sf::Vector2f(0, 0), type,
                                                                                    texture,
                                                                                    sf::IntRect(0, 0, 10, 10)) {
    _Speed = speed;

    if (type == GameObjectType::BulletObjectPlayer) {
        setSpriteColor(sf::Color(225, 0, 0));
    } else if (type == GameObjectType::BulletObjectBoss) {
        setSpriteColor(sf::Color(0, 45, 255));
    } else {
        setSpriteColor(sf::Color(255, 255, 0));
    }
}

void Bullet::update(float FrameTime, int RoadSpeed) {
    _Direction = _Direction / float(std::sqrt(std::pow(_Direction.x, 2) + std::pow(_Direction.y, 2)));
    sf::Vector2f move = _Direction * FrameTime * _Speed;
    setPos(getPos() + move);
}

//void Bullet::playShotSound(sf::Vector2f position, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>> &soundEffects,
//                      sf::SoundBuffer &soundBuffer, float Volume) {
//    std::shared_ptr<sf::Sound> ShotSound(new sf::Sound());
//    ShotSound->setBuffer(soundBuffer);
//    ShotSound->setPosition(position.x, 0.f, position.y);
//    ShotSound->setMinDistance(500.f);
//    ShotSound->setAttenuation(2.f);
//    ShotSound->setVolume(Volume);
//
//    if (soundEffects.size() <= 225 && Volume > 0) {
//        soundEffects.push_back({ShotSound, 0});
//    }
//}

void Bullet::operator>>(sf::Packet &packet) {
    GameObject::operator>>(packet);
    write(packet, _Direction.x);
    write(packet, _Direction.y);
    //write(packet, _Speed);
}

void Bullet::operator<<(sf::Packet &packet) {
    GameObject::operator<<(packet);
    float dx, dy;
    read(packet, dx);
    read(packet, dy);
    _Direction = sf::Vector2f(dx, dy);
    //read(packet, _Speed);
}
