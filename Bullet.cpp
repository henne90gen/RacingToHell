#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet(sf::Vector2f Position, float Direction, int Speed, GameObjects Type) : GameObject(Type, "Bullet"), _Direction(Direction), _Speed(Speed)
{
	setPos(Position);

	if (Type == GameObjects::BulletObjectPlayer)
	{
		setSpriteColor(sf::Color(255, 0, 0));
	}
}

Bullet::~Bullet()
{

}

void Bullet::update(float FrameTime)
{
	setPos(sf::Vector2f(getPos().x + std::cosf(PI * _Direction / 180) * FrameTime * _Speed, getPos().y + std::sinf(PI * _Direction / 180 ) * _Speed * FrameTime));
}

void Bullet::handleEvent(sf::Event& Event)
{
}
