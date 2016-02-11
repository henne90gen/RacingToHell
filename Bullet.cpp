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
	setPos(getPos() + sf::Vector2f(std::cosf(PI * (float)_Direction / 180.0f), std::sinf(PI * (float)_Direction / 180.0f)) * FrameTime * (float)_Speed);
	//std::cout << std::sqrtf(std::powf(std::cosf(PI * _Direction / 180) * FrameTime * _Speed, 2) + std::powf((std::sinf(PI * _Direction / 180) * _Speed * FrameTime), 2)) << std::endl;
	//std::cout << std::cosf(PI * _Direction / 180) << " | " << std::sinf(PI * _Direction / 180) << std::endl;
}

void Bullet::handleEvent(sf::Event& Event)
{


}
