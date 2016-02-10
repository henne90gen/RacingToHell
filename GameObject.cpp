#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::render(sf::RenderWindow& Window) {
	_Sprite.setTexture(_Texture);
	Window.draw(_Sprite);
}