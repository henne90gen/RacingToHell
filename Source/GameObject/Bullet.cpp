#include "stdafx.h"
#include "GameObject\Bullet.h"

Bullet::Bullet(sf::Vector2f& pos, sf::Vector2f& dir, int speed, GameObjectType type, sf::Texture& texture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBuffer, float Volume) : GameObject(pos, type, texture), _Direction(dir), _Speed(speed)
{

	if (type == GameObjectType::BulletObjectPlayer)
	{
		setSpriteColor(sf::Color(225, 0, 0));
	}
	else if (type == GameObjectType::BulletObjectBoss)
	{
		setSpriteColor(sf::Color(0, 45, 255));
	}
	else
	{
		setSpriteColor(sf::Color(255, 255, 0));
	}

	playShotSound(pos, soundEffects, soundBuffer, Volume);
}

void Bullet::update(float FrameTime, int RoadSpeed)
{
	_Direction = _Direction  / std::sqrt(std::pow(_Direction.x, 2) + std::pow(_Direction.y, 2));
	sf::Vector2f move = _Direction * FrameTime * _Speed;
	setPos(getPos() + move);
}


void Bullet::playShotSound(sf::Vector2f position, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBuffer, float Volume)
{
	std::shared_ptr<sf::Sound> ShotSound(new sf::Sound());
	ShotSound->setBuffer(soundBuffer);
	ShotSound->setPosition(position.x, 0.f, position.y);
	ShotSound->setMinDistance(500.f);
	ShotSound->setAttenuation(4.f);
	ShotSound->setVolume(Volume * 2);

	if (soundEffects.size() <= 200)
	{
		soundEffects.push_back({ ShotSound, 0 });
	}
}