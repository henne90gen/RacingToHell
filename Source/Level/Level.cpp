#include "stdafx.h"
#include "Level\Level.h"

Level::Level() : _Difficulty(1), _LevelUp(10)
{
}

Level::~Level()
{

}

// returns true if 'level-up' has happened
bool Level::update(float FrameTime, GameState gameState)
{
	if (_Sprite.getPosition().y + FrameTime * _Difficulty >= 0)
	{
		_Sprite.setPosition(sf::Vector2f(0, -1600));
	}
	else
	{
		_Sprite.setPosition(sf::Vector2f(_Sprite.getPosition().x, _Sprite.getPosition().y + FrameTime * getRoadSpeed()));
	}

	if (gameState == GameState::Running && _Difficulty < 4)
	{
		if (_Timer.getElapsedTime().asSeconds() >= _LevelUp)
		{
			return true;
		}
	}
	return false;
}

void Level::render(sf::RenderWindow& RenderWindow)
{
	RenderWindow.draw(_Sprite);
}

void Level::playMusic() 
{
	if (_Music.getStatus() == sf::Sound::Stopped || _Music.getStatus() == sf::Sound::Paused) {
		_Music.play();
	}
}

void Level::stopMusic() 
{
	_Music.stop();
}

void Level::pauseMusic()
{
	_Music.pause();
}

void Level::setVolume(float Volume)
{
	_Music.setVolume(Volume);
}

void Level::levelUp()
{
	_Timer.restart();
	_Difficulty++;
	if (_Difficulty - 1 < _Textures.size()) {
		_Sprite.setTexture((*_Textures.at(_Difficulty - 1)));
	}
}

void Level::load()
{
	for (int i = 0; i < 4; i++) {
		sf::Texture* texture = new sf::Texture();
		(*texture).loadFromFile("Resources/Texture/Road/Road" + std::to_string(i + 1) + ".jpg");
		_Textures.push_back(texture);
	}
	_Sprite.setTexture((*_Textures.at(0)));
	_Sprite.setPosition(sf::Vector2f(0, -1600));

	if (_MusicBuffer.loadFromFile("Resources/Sound/Music/level1.ogg")) {
		_Music.setBuffer(_MusicBuffer);
	}
}

void Level::resetLevel()
{
	_Difficulty = 1;
	_Sprite.setTexture((*_Textures.at(_Difficulty - 1)));
}
