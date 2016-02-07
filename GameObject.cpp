#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::render(sf::RenderWindow& Window) {
	Window.draw(_Sprite);
}