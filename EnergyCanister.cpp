#include "stdafx.h"
#include "EnergyCanister.h"


EnergyCanister::EnergyCanister(sf::Vector2f pos) : GameObject(GameObjects::Canister, "canister")
{
	setPos(pos);
}


EnergyCanister::~EnergyCanister()
{
}

void EnergyCanister::update(float FrameTime)
{
	setPos(sf::Vector2f(getPos().x, getPos().y + FrameTime * 100));
}

void EnergyCanister::handleEvent(sf::Event & Event)
{
}
