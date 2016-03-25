#include "stdafx.h"
#include "Level\Level.h"

Level::Level() : _Level(1), _LevelTime(1.f) {}

bool Level::update(float FrameTime, GameState gameState)
{
	if (_Sprite.getPosition().y + FrameTime * _Level >= 0)
	{
		_Sprite.setPosition(sf::Vector2f(0, -1600));
	}
	else
	{
		_Sprite.setPosition(sf::Vector2f(_Sprite.getPosition().x, _Sprite.getPosition().y + FrameTime * getRoadSpeed()));
	}

	if (gameState == GameState::Running && _Level < 4)
	{
		if (_Timer.getElapsedTime().asSeconds() >= _LevelTime)
		{
			return true;
		}
	}
	return false;
}

void Level::render(sf::RenderWindow& window)
{
	window.draw(_Sprite);
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
	_Level++;
	if (_Level - 1 < _Textures.size()) {
		_Sprite.setTexture((*_Textures.at(_Level - 1)));
	}
}

void Level::load()
{
	for (int i = 0; i < 4; i++) {
		std::shared_ptr<sf::Texture> texture(new sf::Texture());
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
	_Level = 1;
	_Sprite.setTexture((*_Textures.at(_Level - 1)));
}
