#include "stdafx.h"
#include "GameObject/Boss/Jet.h"

Jet::Jet(unsigned int id, int difficulty, int HP, sf::Texture & texture, sf::Texture & bulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBufferShot, sf::SoundBuffer &soundBufferExplosion, sf::SoundBuffer &soundBufferEngine, float Volume) :
	BossCar(id, sf::Vector2f(-1 * texture.getSize().x, SCREENHEIGHT / 2), difficulty, HP, 500, texture, bulletTexture, soundEffects, soundBufferShot, soundBufferExplosion, Volume),
	_EngineSoundBuffer(soundBufferEngine)
{
	init();
}

Jet::Jet(std::istream & stream, sf::Texture & texture, sf::Texture & bulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer & soundBufferShot, sf::SoundBuffer & soundBufferExplosion, sf::SoundBuffer & soundBufferEngine, float volume) :
	BossCar(stream, texture, bulletTexture, soundEffects, soundBufferShot, soundBufferExplosion, volume)
{
	init();
}

Jet::Jet(sf::Packet & packet, sf::Texture & texture, sf::Texture & bulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer & soundBufferShot, sf::SoundBuffer & soundBufferExplosion, sf::SoundBuffer & soundBufferEngine, float volume) :
	BossCar(packet, texture, bulletTexture, soundEffects, soundBufferShot, soundBufferExplosion, volume)
{
	init();
}

void Jet::render(sf::RenderWindow & window)
{
	window.draw(getSprite());

	window.draw(_HealthBar);
	window.draw(_HealthBarFrame);

	renderExplosions(window);
}

void Jet::update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<Bullet>>& bullets, PlayerCar& player)
{
	if (!_IsExploding) {
		if (_Movement != Movement::STILL) {
			if (driveToNextPosition(frameTime)) {
				//_BossEventTimer1.restart();
				//_BossEventTimer2.restart();
				//_Event1Switch = false;
				//_Event2Switch = false;
				//_Event2Counter = 0;
				//_Event1Counter = 0;

				_Movement = Movement::STILL;
			}
			else {
				_EngineSound.setPosition(getPos().x, 0.f, getPos().y);
				if (_EngineSound.getStatus() == sf::Sound::Paused || _EngineSound.getStatus() == sf::Sound::Stopped) {
					_EngineSound.play();
				}
			}
		}
		else if (_Movement == Movement::STILL) {
			if (_EngineSound.getStatus() == sf::Sound::Playing) _EngineSound.stop();

			switch (_Pattern[_CurrentPhase].first)
			{
			case Phase::SIDE:
			{
				_Event1Frequency = 0.2f;

				if (getBossEvent() == 1)
				{
					for (int i = -40; i <= SCREENHEIGHT; i += 200)
					{
						BossCar::shootBullet(bullets, sf::Vector2f(0, i), sf::Vector2f(1, 0));
						BossCar::shootBullet(bullets, sf::Vector2f(SCREENWIDTH, i + 100), sf::Vector2f(-1, 0));
					}
				}
				break;
			}
			case Phase::SAVELANES:
			{
				_Event1Frequency = 0.2f;
				_Event2Frequency = 0.0f;

				if (getBossEvent() == 1 || _Event2Counter > 0)
				{
					_Event2Frequency = 3.0f + 1.0f * (float)_Difficulty;

					if (getBossEvent() == 2)
					{
						if (_Event2Counter + 1 <= 3 + 2 * _Difficulty)
						{
							for (int i = 0; i < 3; i++)
							{
									BossCar::shootBullet(bullets, sf::Vector2f(i * 150 + 150, 0), sf::Vector2f(0, 1));
							}

								BossCar::shootBullet(bullets, sf::Vector2f(20, 0), sf::Vector2f(0, 1));
								BossCar::shootBullet(bullets, sf::Vector2f(SCREENWIDTH - 20, 0), sf::Vector2f(0, 1));

							++_Event2Counter;
						}
						else
						{
							_Event2Counter = 0;
							_Event2Frequency = 0.0f;
						}
					}
				}
			}
			default:
				break;
			}
		}

		if (_Movement == Movement::PARABOLA)
		{
			setPos(getPos() + sf::Vector2f(0, (float)roadSpeed * frameTime));
		}
	
		checkPhase();
		updateHealthBar();
	}
	else {
		updateExplosions(frameTime);
	}
}

void Jet::shootBullet(std::vector<std::shared_ptr<Bullet>>& bullets, sf::Vector2f pos, sf::Vector2f dir, int bulletSpeed, float volume)
{
	bullets.push_back(GameObjectFactory::getBullet(pos, dir, bulletSpeed, GameObjectType::BulletObjectBoss, _soundEffects, volume));
}

void Jet::randomPosition()
{
	bool LeftRight = (std::rand() % 100) > 50;

	setPos(sf::Vector2f(-0.5f * getWidth() + std::pow(-1, (int)!LeftRight) * (SCREENWIDTH * (int)LeftRight + getWidth() * 4), 0.5f * getHeight() + ((std::rand() % 100) / 100.0f) * (SCREENHEIGHT - getHeight())));
	getSprite().setRotation(-180.0f * (int)(LeftRight));

	_NextPosition = sf::Vector2f(std::pow(-1, (int)LeftRight) * (SCREENWIDTH * (int)!LeftRight + getWidth() * 4), getPos().y);
}

void Jet::checkPhase()
{
	if (_PhaseClock.getElapsedTime().asSeconds() >= _Pattern[_CurrentPhase].second) {
		if (_CurrentPhase + 1 >= _Pattern.size()) {
			_CurrentPhase = 0;
		}
		else {
			++_CurrentPhase;
		}

		_Movement = Movement::STRAIGHT;
		randomPosition();
		_PhaseClock.restart();
	}
}

void Jet::stopSounds()
{
	_EngineSound.stop();
}

void Jet::init()
{
	_EngineSound.setBuffer(_EngineSoundBuffer);
	_EngineSound.setVolume(_Volume * 5.5f);
	_EngineSound.setPosition(getPos().x, 0.f, getPos().y);
	_EngineSound.setMinDistance(600.f);
	_EngineSound.setAttenuation(8.f);

	_Traffic = true;
	_Pattern = { std::make_pair(Phase::SIDE, 6.0f) };
	_CurrentPhase = 0;

	randomPosition();

	_Speed = (0.8f + 0.2f * (float)_Difficulty) * _Speed;

	_Movement = Movement::STRAIGHT;

	_Pattern = { std::make_pair(Phase::SIDE, 10.5f), std::make_pair(Phase::SAVELANES, 9.f) };
}
