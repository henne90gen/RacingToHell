#include "stdafx.h"
#include "Level\Level.h"

Level::Level() : _Difficulty(300)
{
	_Texture.loadFromFile("Resources/Texture/Road/Road1.jpg");
	_Sprite.setTexture(_Texture);
	_Sprite.setPosition(sf::Vector2f(0, - 1600));
}

Level::~Level()
{

}

void Level::update(float FrameTime)
{
	if (_Sprite.getPosition().y + FrameTime * _Difficulty >= 0)
	{
		_Sprite.setPosition(sf::Vector2f(0, -1600));
	}
	else
	{
		_Sprite.setPosition(sf::Vector2f(_Sprite.getPosition().x, _Sprite.getPosition().y + FrameTime * _Difficulty));
	}
}

void Level::render(sf::RenderWindow& RenderWindow)
{
	RenderWindow.draw(_Sprite);
}