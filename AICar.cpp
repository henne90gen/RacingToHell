#include "stdafx.h"
#include "AICar.h"


AICar::AICar() : Car(100, std::rand() % 100 + 100, "car") 
{
	setPos(sf::Vector2f(std::rand() % 4 * SCREENWIDTH / 4 + SCREENWIDTH / 8, 0));
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
