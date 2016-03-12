#include "stdafx.h"
#include "GameObject/Boss/Mech.h"


Mech::Mech(sf::Texture& TextureTop, sf::Texture& TextureLegs, sf::Texture* BulletTexture) : BossCar(sf::Vector2f(200, 200), 2000, 400, TextureTop, BulletTexture), 
	_TopAnim(sf::Vector2f(200, 200), TextureTop), _LegsAnim(sf::Vector2f(200, 200), TextureLegs)
{
	setSprite(_TopAnim.getSprite());
}

Mech::~Mech()
{
}

void Mech::render(sf::RenderWindow & Window)
{
	_LegsAnim.render(Window);
	_TopAnim.render(Window);
}

void Mech::update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects)
{
	_LegsAnim.update(FrameTime);
	_TopAnim.update(FrameTime);
}
