#include "stdafx.h"
#include "Animation/Explosion.h"


Explosion::Explosion(sf::Vector2f pos, sf::Texture & texture, sf::Vector2f movement, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBuffer, float Volume)
	: Animation(pos, texture, 0.0075, 81, 9, 9)
{
	_Sprite.setScale(sf::Vector2f(1.1f, 1.1f));
	_Movement = movement;

	playExplosionSound(pos, soundEffects, soundBuffer, Volume);
}

void Explosion::render(sf::RenderWindow & window)
{
	if (_AnimState == Animation::Play || _AnimState == Animation::Pause)
		window.draw(_Sprite);
}

void Explosion::update(float FrameTime)
{
	if (_AnimState == Animation::Play) {
		_Sprite.setPosition(_Sprite.getPosition() + _Movement * FrameTime);
		if (_Timer.getElapsedTime().asSeconds() > _TimePerFrame) {
			_ElapsedTime += _Timer.getElapsedTime().asSeconds();
			_Timer.restart();
			if (!nextFrame()) {
				_AnimState = Animation::Stop;
			}
		}
	}
}

void Explosion::playExplosionSound(sf::Vector2f position, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBuffer, float Volume)
{
	std::shared_ptr<sf::Sound> explosionSound(new sf::Sound());
	explosionSound->setBuffer(soundBuffer);
	explosionSound->setPosition(position.x, 0.f, position.y);
	explosionSound->setMinDistance(500.f);
	explosionSound->setAttenuation(4.f);
	explosionSound->setVolume(Volume * 5.f);

	soundEffects.push_back({ explosionSound, 0 });
}
