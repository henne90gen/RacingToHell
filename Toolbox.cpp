#include "stdafx.h"
#include "Toolbox.h"


Toolbox::Toolbox(sf::Vector2f pos) : GameObject(pos, GameObjects::Tools, "toolbox")
{
}


Toolbox::~Toolbox()
{
}

void Toolbox::update(float FrameTime)
{
	setPos(sf::Vector2f(getPos().x, getPos().y + FrameTime * 100));
}

void Toolbox::handleEvent(sf::Event & Event)
{
}
