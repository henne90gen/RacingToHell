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

void GameObject::resetMovement() {
	
}