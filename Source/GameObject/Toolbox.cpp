#include "stdafx.h"
#include "GameObject\Toolbox.h"


Toolbox::Toolbox(sf::Vector2f pos, sf::Texture& texture) : GameObject(pos, GameObjects::Tools, texture)
{
}


Toolbox::~Toolbox()
{
}

void Toolbox::update(float FrameTime, int RoadSpeed)
{
	setPos(sf::Vector2f(getPos().x, getPos().y + FrameTime * RoadSpeed));
}

void Toolbox::handleEvent(sf::Event & Event)
{
}
