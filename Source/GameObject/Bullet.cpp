#include "stdafx.h"
#include "GameObject/Bullet.h"

Bullet::Bullet(unsigned int id, GameObjectManager &gom, sf::Vector2f pos, sf::Vector2f dir, float speed,
               GameObjectType type,
               sf::Texture &texture) : GameObject(id, gom, type, pos, texture, sf::IntRect(0, 0, 10, 10)) {

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
    _Movement = rh::normalize(dir) * speed;
}

//Bullet::Bullet(sf::Packet &packet, GameObjectType type, sf::Texture &texture) :
//        GameObject(packet, type, texture, sf::IntRect(0, 0, 10, 10)) {
//    Bullet::operator<<(packet);
//    if (type == GameObjectType::BulletObjectPlayer) {
//        setSpriteColor(sf::Color(225, 0, 0));
//    } else if (type == GameObjectType::BulletObjectBoss) {
//        setSpriteColor(sf::Color(0, 45, 255));
//    } else {
//        setSpriteColor(sf::Color(255, 255, 0));
//    }
//    playShotSound(getPosition(), soundEffects, soundBuffer, Volume);
//}

//Bullet::Bullet(sf::Packet &packet, sf::Vector2f PlayerPosition, GameObjectType type, sf::Texture &texture,
//               sf::Uint32 ID, float speed) {
//    _ID = ID;
//    _Type = type;
//    initTexture(texture, sf::IntRect(0, 0, 10, 10));
//    setPos(PlayerPosition);
//
//    float dx, dy;
//    packet >> dx >> dy;
//
//    _Movement = rh::normalize(sf::Vector2f(dx, dy) - getPosition()) * speed;
//
//    if (type == GameObjectType::BulletObjectPlayer) {
//        setSpriteColor(sf::Color(225, 0, 0));
//    } else if (type == GameObjectType::BulletObjectBoss) {
//        setSpriteColor(sf::Color(0, 45, 255));
//    } else {
//        setSpriteColor(sf::Color(255, 255, 0));
//    }
//}

//Bullet::Bullet(GameObjectType type, sf::Texture &texture, float speed) : GameObject(0, type, sf::Vector2f(0, 0),
//                                                                                    texture, sf::IntRect(0, 0, 10, 10)) {
//    _Movement = rh::normalize(sf::Vector2f(dx, dy) - getPosition()) * speed;
//
//    if (type == GameObjectType::BulletObjectPlayer) {
//        setSpriteColor(sf::Color(225, 0, 0));
//    } else if (type == GameObjectType::BulletObjectBoss) {
//        setSpriteColor(sf::Color(0, 45, 255));
//    } else {
//        setSpriteColor(sf::Color(255, 255, 0));
//    }
//}

void Bullet::operator>>(sf::Packet &packet) {
    GameObject::operator>>(packet);
//    write(packet, _Direction.x);
//    write(packet, _Direction.y);
    //write(packet, _Speed);
}

void Bullet::operator<<(sf::Packet &packet) {
    GameObject::operator<<(packet);
    float dx, dy;
    read(packet, dx);
    read(packet, dy);
//    _Direction = sf::Vector2f(dx, dy);
    //read(packet, _Speed);
}
