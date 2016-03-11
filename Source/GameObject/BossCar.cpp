#include "stdafx.h"
#include "GameObject\BossCar.h"

BossCar::BossCar(sf::Vector2f& Position, int Health, float Speed, sf::Texture& Texture) : Car(Position, Health, Speed, GameObjects::Boss, Texture)
{
	
}

BossCar::~BossCar()
{

}

void BossCar::update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects)
{
}

void BossCar::render(sf::RenderWindow & RenderWindow)
{
}
