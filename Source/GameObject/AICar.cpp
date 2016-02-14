#include "stdafx.h"
#include "GameObject\AICar.h"


AICar::AICar() : Car(sf::Vector2f(0, 0), 100, std::rand() % 100 + 100, GameObjects::AI, "Traffic" + std::to_string(std::rand() % 7 + 1))
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

void AICar::update(float FrameTime)
{
	setPos(sf::Vector2f(getPos().x, getPos().y + _Speed * FrameTime));
}
