#include "stdafx.h"
#include "Animation/Animation.h"


Animation::Animation(sf::Vector2f pos, std::string path, float timePerFrame) : _AnimState(Animation::Play), _TimePerFrame(timePerFrame)
{
	if (_SpriteSheet.loadFromFile(path)) {
		_Sprite.setTexture(_SpriteSheet);
		_CurrentSprite = sf::IntRect(0, 0, 100, 100);
		_Sprite.setTextureRect(_CurrentSprite);
		_Sprite.setOrigin(_Sprite.getLocalBounds().width / 2.0f, _Sprite.getLocalBounds().height / 2.0f);
		_Sprite.setPosition(pos);
	}
}


Animation::~Animation()
{
}

bool Animation::nextSprite()
{
	_CurrentSprite.left += 100;
	if (_CurrentSprite.left == 900) {
		_CurrentSprite.left = 0;
		_CurrentSprite.top += 100;
		if (_CurrentSprite.top == 900) {
			return false;
		}
	}
	_Sprite.setTextureRect(_CurrentSprite);
	return true;
}
