#include "stdafx.h"
#include "GameObject\GameObject.h"


GameObject::GameObject(sf::Vector2f pos, GameObjects Type, sf::Texture& texture) : _Type(Type)
{
	//Setting up texture and sprite
	_Texture = texture;
	_Sprite.setTexture(_Texture);
	_Sprite.setOrigin(_Sprite.getLocalBounds().width / 2, _Sprite.getLocalBounds().height / 2);
	_Sprite.setPosition(pos);
}

GameObject::~GameObject()
{

}

void GameObject::render(sf::RenderWindow& Window)
{
	Window.draw(_Sprite);
}

void GameObject::setSkin(sf::Texture* skin)
{
	_Texture = *skin;
	_Sprite.setTexture(_Texture);
}

bool GameObject::checkForCollision(GameObject * go)
{
	return Collision::BoundingBoxTest(getSprite(), go->getSprite());
}

