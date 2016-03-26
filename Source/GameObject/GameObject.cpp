#include "stdafx.h"
#include "GameObject\GameObject.h"


GameObject::GameObject(sf::Vector2f pos, GameObjectType type, sf::Texture& texture) : _Type(type)
{
	//Setting up texture and sprite
	_Texture = texture;
	_Sprite.setTexture(_Texture);
	_Sprite.setOrigin(_Sprite.getLocalBounds().width / 2, _Sprite.getLocalBounds().height / 2);
	_Sprite.setPosition(pos);
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
	write(stream, getType());
	write(stream, getPos().x);
	write(stream, getPos().y);
}

void GameObject::operator<<(std::istream& stream)
{
	float x, y;
	read(stream, x);
	read(stream, y);
	setPos(sf::Vector2f(x, y));
}