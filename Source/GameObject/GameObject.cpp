#include "stdafx.h"
#include "GameObject\GameObject.h"


GameObject::GameObject(unsigned int id, sf::Vector2f pos, GameObjectType type, sf::Texture& texture) : _ID(id), _Type(type)
{
	initTexture(texture);
	_Sprite.setPosition(pos);
}

GameObject::GameObject(std::istream& stream, GameObjectType type, sf::Texture& texture) : _Type(type)
{
	if (_Type == GameObjectType::Canister || _Type == GameObjectType::Tools) {
		GameObject::operator<<(stream);
	}
	initTexture(texture);
	_Sprite.setPosition(getPos());
}

GameObject::GameObject(std::istream& stream, GameObjectType type) : _Type(type)
{
	if (_Type == GameObjectType::Canister || _Type == GameObjectType::Tools) {
		GameObject::operator<<(stream);
	}
	_Sprite.setPosition(getPos());
}

GameObject::GameObject(sf::Packet& packet, GameObjectType type, sf::Texture& texture) : _Type(type)
{
	if (_Type == GameObjectType::Canister || _Type == GameObjectType::Tools) {
		GameObject::operator<<(packet);
	}
	initTexture(texture);
	_Sprite.setPosition(getPos());
}

GameObject::GameObject(sf::Packet& packet, GameObjectType type) : _Type(type)
{
	if (_Type == GameObjectType::Canister || _Type == GameObjectType::Tools) {
		GameObject::operator<<(packet);
	}
	_Sprite.setPosition(getPos());
}

void GameObject::render(sf::RenderWindow& window)
{
	window.draw(_Sprite);
}

void GameObject::update(float frameTime, int roadSpeed) {
	setPos(sf::Vector2f(getPos().x, getPos().y + frameTime * roadSpeed));
}

void GameObject::setTexture(sf::Texture& texture)
{
	_Texture = texture;
	_Sprite.setTexture(_Texture);
}

bool GameObject::checkForCollision(GameObject &go)
{
	return Collision::BoundingBoxTest(getSprite(), go.getSprite());
}

float GameObject::getAngleFromVector(sf::Vector2f vec) 
{
	float angle = std::atan(vec.y / vec.x) * 180.0f / PI;
	if (vec.x < 0) {
		angle += 180;
	}
	return angle;
}

sf::Vector2f GameObject::divideByLength(sf::Vector2f vec) 
{
	return vec / std::sqrtf(std::powf(vec.x, 2) + std::powf(vec.y, 2));
}

void GameObject::resetMovement() {
	
}

void GameObject::operator>>(std::ostream& stream)
{
	write(stream, (sf::Uint8)getType());
	write(stream, _ID);
	write(stream, getPos().x);
	write(stream, getPos().y);
}

void GameObject::operator<<(std::istream& stream)
{
	read(stream, _ID);
	float x, y;
	read(stream, x);
	read(stream, y);
	setPos(sf::Vector2f(x, y));
}

void GameObject::operator>>(sf::Packet& packet) {
	write(packet, (sf::Uint8)getType());
	write(packet, _ID);
	write(packet, getPos().x);
	write(packet, getPos().y);
}

void GameObject::operator<<(sf::Packet& packet)
{
	read(packet, _ID);
	float x, y;
	read(packet, x);
	read(packet, y);
	setPos(sf::Vector2f(x, y));
}

void GameObject::initTexture(sf::Texture& texture)
{
	_Texture = texture;
	_Sprite.setTexture(_Texture);
	_Sprite.setOrigin(_Sprite.getLocalBounds().width / 2, _Sprite.getLocalBounds().height / 2);
}