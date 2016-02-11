#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet(sf::Vector2f Position, float Direction, int Speed) : GameObject(GameObjects::BulletObject, "Bullet")
{
	setPos(Position);
	_Direction = Direction;
	_Speed = Speed;
}

Bullet::~Bullet()
{

}

void Bullet::update(float FrameTime)
{
	setPos(sf::Vector2f(getPos().x + std::cosf(PI * _Direction / 180) * FrameTime * _Speed, getPos().y + std::sinf(PI * _Direction / 180 ) * _Speed * FrameTime));
	//std::cout << std::cos(PI * _Direction / 180) << " | " << std::sin(PI * _Direction / 180) << std::endl;
}

void Bullet::handleEvent(sf::Event& Event)
{


}
