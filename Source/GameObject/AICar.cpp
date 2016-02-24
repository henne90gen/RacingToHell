#include "stdafx.h"
#include "GameObject\AICar.h"


AICar::AICar(int RoadSpeed) : Car(sf::Vector2f(0, 0), 100, std::rand() % ((int)(RoadSpeed / 3)) + ((int)(RoadSpeed / 3)), GameObjects::AI, "Traffic" + std::to_string(std::rand() % 7 + 1))
{
	_Lane = std::rand() % 4;
	setPos(sf::Vector2f(_Lane * SCREENWIDTH / 4 + SCREENWIDTH / 8, -getHeight() /2));

}


AICar::~AICar()
{
}

void AICar::handleEvent(sf::Event & Event)
{

}

void AICar::update(float FrameTime, int RoadSpeed)
{
	setPos(sf::Vector2f(getPos().x, getPos().y + (RoadSpeed - _Speed) * FrameTime));
}
