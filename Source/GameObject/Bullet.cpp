#include "stdafx.h"
#include "GameObject\Bullet.h"

Bullet::Bullet(sf::Vector2f pos, float Direction, int Speed, GameObjectType Type, sf::Texture& texture) : GameObject(pos, Type, texture), _Direction(Direction), _Speed(Speed)
{

	if (Type == GameObjectType::BulletObjectPlayer)
	{
		setSpriteColor(sf::Color(225, 0, 0));
	}
	else if (Type == GameObjectType::BulletObjectBoss)
	{
		setSpriteColor(sf::Color(0, 45, 255));
	}
	else
	{
		setSpriteColor(sf::Color(255, 255, 0));
	}
}

void Bullet::update(float FrameTime, int RoadSpeed)
{
	sf::Vector2f move = sf::Vector2f(std::cosf(PI * _Direction / 180.0f), std::sinf(PI * _Direction / 180.0f)) * FrameTime * _Speed;
	setPos(getPos() + move);
}
