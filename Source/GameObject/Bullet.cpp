#include "stdafx.h"
#include "GameObject\Bullet.h"

Bullet::Bullet(sf::Vector2f pos, float Direction, int Speed, GameObjects Type) : GameObject(pos, Type, "Bullet"), _Direction(Direction), _Speed(Speed)
{

	if (Type == GameObjects::BulletObjectPlayer)
	{
		setSpriteColor(sf::Color(255, 0, 0));
	}
}

Bullet::~Bullet()
{

}

void Bullet::update(float FrameTime, int RoadSpeed)
{
	//Move ist immer gleich lang, kann also nicht der Grund für unterschiedlich schnelle Bullets sein
	sf::Vector2f move = sf::Vector2f(std::cosf(PI * _Direction / 180.0f), std::sinf(PI * _Direction / 180.0f)) * FrameTime * _Speed;
	setPos(getPos() + move);
	//std::cout << std::sqrtf(std::powf(std::cosf(PI * _Direction / 180) * FrameTime * _Speed, 2) + std::powf((std::sinf(PI * _Direction / 180) * _Speed * FrameTime), 2)) << std::endl;
	//std::cout << std::cosf(PI * _Direction / 180) << " | " << std::sinf(PI * _Direction / 180) << std::endl;
	//std::cout << std::sqrtf(move.x * move.x + move.y * move.y) << std::endl;
}

void Bullet::handleEvent(sf::Event& Event)
{
}
