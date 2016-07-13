#include "stdafx.h"
#include "Level/Level.h"

bool Level::update(float FrameTime, GameState gameState)
{
	_LevelTime += FrameTime;

	if (_Sprite.getPosition().y + FrameTime * _Level >= 0)
	{
		_Sprite.setPosition(sf::Vector2f(0, -1600));
	}
	else
	{
		_Sprite.setPosition(sf::Vector2f(_Sprite.getPosition().x, _Sprite.getPosition().y + FrameTime * getRoadSpeed()));
	}

	if (gameState == GameState::Running)
	{
		if (_LevelTime >= _TotalLevelTime)
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
	_LevelTime = 0;
	_Level++;
	_Sprite.setTexture((*_Textures.at((_Level - 1) % _Textures.size())));
	_Music.setBuffer((*_MusicBuffers.at((_Level - 1) % _Textures.size())));
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

	std::thread loadingThreads[5];
	for (int i = 1; i <= 5; i++) loadingThreads[i - 1] = (std::thread(&Level::loadSongByID, this, i));
	for (auto& t: loadingThreads) t.join();
}

void Level::resetLevel()
{
	// TODO set _TotalLevelTime back to 60
	_Level = 1;
	_TotalLevelTime = 5.0f;
	_LevelTime = 0;
	_Music.setBuffer((*_MusicBuffers.at(0)));
	_Sprite.setTexture((*_Textures.at((_Level - 1) % _Textures.size())));
}

int Level::getRoadSpeed()
{
	switch (_Difficulty)
	{
	case 0:
	case 1:
	case 2:
		return (60 + _Difficulty * 20) * _Level + 100;
		break;
	case 3:
		return (110 * _Level + 150);
		break;
	default:
		return -100;
		break;
	}
}

void Level::loadSongByID(int id)
{
	try {
		#ifdef SFML_SYSTEM_WINDOWS
			bool checked = false;
			while (!checked) {
				std::shared_ptr<sf::SoundBuffer> buffer(new sf::SoundBuffer());

				if (_CrtIsValidHeapPointer((const void *)buffer.get())) {
					(*buffer).loadFromFile("Resources/Sound/Music/level" + std::to_string(id) + ".ogg");
				std::lock_guard<std::mutex> lock(_ThreadGuard);
					_MusicBuffers.push_back(buffer);
					checked = true;
				}
				else {
					buffer.reset();
				}
			}
		#else
			std::shared_ptr<sf::SoundBuffer> buffer(new sf::SoundBuffer());

			(*buffer).loadFromFile("Resources/Sound/Music/level" + std::to_string(id) + ".ogg");
			std::lock_guard<std::mutex>{ _ThreadGuard };
			_MusicBuffers.push_back(buffer);
		#endif
	}
	catch (...) {
		std::exit(1);
	}
}
