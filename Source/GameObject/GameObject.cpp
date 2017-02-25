#include "stdafx.h"
#include "GameObject/GameObject.h"


GameObject::GameObject(unsigned int id, GameObjectType type, sf::Vector2f pos, sf::Texture &texture,
                       sf::IntRect textureRect)
        : _ID(id), _Type(type), _Position(pos), _Movement(sf::Vector2f(0, 0)) {
    initTexture(texture, textureRect);
}

GameObject::GameObject(sf::Packet &packet, GameObjectType type, sf::Texture &texture, sf::IntRect textureRect)
        : _Type(type) {
    if (_Type == GameObjectType::Canister || _Type == GameObjectType::Tools) {
        GameObject::operator<<(packet);
    }
    initTexture(texture, textureRect);
}

GameObject::GameObject(sf::Packet &packet, GameObjectType type) : _Type(type) {
    if (_Type == GameObjectType::Canister || _Type == GameObjectType::Tools) {
        GameObject::operator<<(packet);
    }
}

void GameObject::render(sf::RenderWindow &window) {
    window.draw(_Sprite);
}

void GameObject::update(float frameTime) {
    _Position += _Movement * frameTime;
    _Sprite.setPosition(_Position);
}

void GameObject::setTexture(sf::Texture &texture) {
    _Texture = texture;
    _Sprite.setTexture(_Texture);
}

bool GameObject::checkForCollision(GameObject &go) {
    return Collision::BoundingBoxTest(getSprite(), go.getSprite());
}

void GameObject::operator>>(sf::Packet &packet) {
    //write(packet, (sf::Uint8)getType());
    write(packet, (sf::Uint32) getID());
    write(packet, _Position.x);
    write(packet, _Position.y);
}

void GameObject::operator<<(sf::Packet &packet) {
    //sf::Uint8 type;
    //read(packet, type);
    //_Type = (GameObjectType) type;
    read(packet, _ID);
    float x, y;
    read(packet, x);
    read(packet, y);
    _Position = sf::Vector2f(x, y);
}

void GameObject::initTexture(sf::Texture &texture, sf::IntRect textureRect) {
    _Texture = texture;
    _Sprite.setTexture(_Texture);
    _Sprite.setTextureRect(textureRect);
    _Sprite.setOrigin(_Sprite.getLocalBounds().width / 2, _Sprite.getLocalBounds().height / 2);
}
