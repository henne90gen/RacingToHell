#include "stdafx.h"
#include "Level\Level.h"

Level::Level() : _Difficulty(1), _LevelUp(1) {}

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
	_Difficulty++;
	if (_Difficulty - 1 < _Textures.size()) {
		_Sprite.setTexture((*_Textures.at(_Difficulty - 1)));
		_Music.setBuffer((*_MusicBuffers.at(_Difficulty - 1)));
	}
}

void Level::load()
{
	for (int i = 1; i <= 4; i++) {
		std::shared_ptr<sf::Texture> texture(new sf::Texture());
		(*texture).loadFromFile("Resources/Texture/Road/Road" + std::to_string(i) + ".jpg");
		_Textures.push_back(texture);
	}
	_Sprite.setTexture((*_Textures.at(0)));
	_Sprite.setPosition(sf::Vector2f(0, -1600));

	for (int i = 1; i <= 5; i++) {
		std::shared_ptr<sf::SoundBuffer> buffer(new sf::SoundBuffer());
		//(*buffer).loadFromFile("Resources/Sound/Music/level" + std::to_string(i) + ".ogg");
		_MusicBuffers.push_back(buffer);
	}
	_Music.setBuffer((*_MusicBuffers.at(0)));
}

void Level::resetLevel()
{
	_Difficulty = 1;
	_Sprite.setTexture((*_Textures.at(_Difficulty - 1)));
}
