#include "stdafx.h"
#include "GameObject/BossCar.h"

BossCar::BossCar(std::pair<sf::Texture*, sf::Texture*> textures) : Car(sf::Vector2f(50, 50), 1000, 500, GameObjects::Boss, (*textures.first))
{
	_CannonTexture = (*textures.second);
	_CannonSprite.setTexture(_CannonTexture);
	_CannonSprite.setOrigin(_CannonSprite.getLocalBounds().width / 2.0f, 50.0f);
}

BossCar::~BossCar()
{
}

void BossCar::render(sf::RenderWindow& Window) {
	Window.draw(getSprite());
	Window.draw(_CannonSprite);
}

void BossCar::update(float FrameTime, int RoadSpeed)
{
	_CannonSprite.setPosition(getPos());
}
