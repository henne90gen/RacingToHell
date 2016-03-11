#include "stdafx.h"
#include "Animation/Explosion.h"


Explosion::Explosion(sf::Vector2f pos) : Animation(pos, "Resources/Texture/Animation/explosion.png", 0.01)
{
}


Explosion::~Explosion()
{
}

void Explosion::render(sf::RenderWindow & Window)
{
	if (_AnimState == Animation::Play || _AnimState == Animation::Pause)
		Window.draw(_Sprite);
}

void Explosion::update(float FrameTime)
{
	if (_AnimState == Animation::Play) {
		if (_Timer.getElapsedTime().asSeconds() > _TimePerFrame) {
			_ElapsedTime += _Timer.getElapsedTime().asSeconds();
			_Timer.restart();
			if (!nextSprite()) {
				_AnimState = Animation::Stop;
			}
		}
	}
}
