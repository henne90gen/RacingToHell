#include "stdafx.h"
#include "GameObject\EnergyCanister.h"


EnergyCanister::EnergyCanister(sf::Vector2f pos) : GameObject(pos, GameObjects::Canister, "canister")
{
}


EnergyCanister::~EnergyCanister()
{
}

void EnergyCanister::update(float FrameTime, int RoadSpeed)
{
	setPos(sf::Vector2f(getPos().x, getPos().y + FrameTime * RoadSpeed));
}

void EnergyCanister::handleEvent(sf::Event & Event)
{
}
