#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject(GameObjects Type, std::string Filename)
{
	_Texture.loadFromFile("Resources/" + Filename + ".png");
	_Sprite.setTexture(_Texture);
	_Sprite.setOrigin(_Sprite.getLocalBounds().width / 2, _Sprite.getLocalBounds().height / 2);

	_Type = Type;
}

GameObject::~GameObject()
{

}

void GameObject::render(sf::RenderWindow& Window)
{
	Window.draw(_Sprite);
}