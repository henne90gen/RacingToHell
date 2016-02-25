#include "stdafx.h"
#include "Level\Level.h"

Level::Level() : _Difficulty(1), _LevelUp(5)
{
	for (int i = 0; i < 2; i++) {
		sf::Texture* texture = new sf::Texture();
		(*texture).loadFromFile("Resources/Texture/Road/Road" + std::to_string(i + 1) + ".jpg");
		_Textures.push_back(texture);
	}
	_Sprite.setTexture((*_Textures.at(0)));
	_Sprite.setPosition(sf::Vector2f(0, - 1600));
}

Level::~Level()
{

}

void Level::update(float FrameTime, bool isRunning)
{
	if (_Sprite.getPosition().y + FrameTime * _Difficulty >= 0)
	{
		_Sprite.setPosition(sf::Vector2f(0, -1600));
	}
	else
	{
		_Sprite.setPosition(sf::Vector2f(_Sprite.getPosition().x, _Sprite.getPosition().y + FrameTime * getRoadSpeed()));
	}

	if (isRunning)
	{
		if (_TimePassed + FrameTime >= _LevelUp)
		{
			_TimePassed = _TimePassed + FrameTime - _LevelUp;
			LevelUp();
		}
		else
		{
			_TimePassed += FrameTime;
		}
	}
}

void Level::render(sf::RenderWindow& RenderWindow)
{
	RenderWindow.draw(_Sprite);
}

void Level::LevelUp()
{
	_Difficulty++;
	if (_Difficulty < 2) {
		_Sprite.setTexture((*_Textures.at(_Difficulty - 1)));
	}
}