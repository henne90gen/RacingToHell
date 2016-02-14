#include "stdafx.h"
#include "EnergyCanister.h"


EnergyCanister::EnergyCanister(sf::Vector2f pos) : GameObject(pos, GameObjects::Canister, "canister")
{
}


EnergyCanister::~EnergyCanister()
{
}

void EnergyCanister::update(float FrameTime)
{
	setPos(sf::Vector2f(getPos().x, getPos().y + FrameTime * 300));
}

void EnergyCanister::handleEvent(sf::Event & Event)
{
}
