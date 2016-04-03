#include "stdafx.h"
#include "GameObject\Bullet.h"

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f dir, int speed, GameObjectType type, sf::Texture& texture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBuffer, float Volume) :
	GameObject(pos, type, texture), _Direction(dir), _Speed(speed)
{

	if (type == GameObjectType::BulletObjectPlayer)
	{
		setSpriteColor(sf::Color(225, 0, 0));
		playShotSound(pos, soundEffects, soundBuffer, Volume * 4.f);
	}
	else if (type == GameObjectType::BulletObjectBoss)
	{
		setSpriteColor(sf::Color(0, 45, 255));
		playShotSound(pos, soundEffects, soundBuffer, Volume * 4.f);
	}
	else
	{
		setSpriteColor(sf::Color(255, 255, 0));
		playShotSound(pos, soundEffects, soundBuffer, Volume * 5.5f);
	}


}

Bullet::Bullet(std::istream& stream, GameObjectType type, sf::Texture& texture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBuffer, float Volume) : 
	GameObject(stream, type, texture)
{
	Bullet::operator<<(stream);
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
	playShotSound(getPos(), soundEffects, soundBuffer, Volume);
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
	ShotSound->setAttenuation(2.f);
	ShotSound->setVolume(Volume);

	if (soundEffects.size() <= 225 && Volume > 0)
	{
		soundEffects.push_back({ ShotSound, 0 });
	}
}

void Bullet::operator>>(std::ostream& stream)
{
	GameObject::operator>>(stream);
	write(stream, _Direction.x);
	write(stream, _Direction.y);
	write(stream, _Speed);
}

void Bullet::operator<<(std::istream& stream)
{
	float dx, dy;
	read(stream, dx);
	read(stream, dy);
	_Direction = sf::Vector2f(dx, dy);
	read(stream, _Speed);
}