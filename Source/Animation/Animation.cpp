#include "stdafx.h"
#include "Animation/Animation.h"


Animation::Animation(sf::Vector2f pos, sf::Texture & texture, float timePerFrame, int frames, int rows, int cols) : 
	_AnimState(Animation::Play), _TimePerFrame(timePerFrame), _NumFrames(frames), _NumRows(rows), _NumCols(cols)
{
	_SpriteSheet = texture;
	_Sprite.setTexture(_SpriteSheet);
	_CurrentSprite = sf::IntRect(0, 0, _Sprite.getLocalBounds().width / _NumCols, _Sprite.getLocalBounds().height / _NumRows);
	_Sprite.setTextureRect(_CurrentSprite);
	_Sprite.setOrigin(_Sprite.getLocalBounds().width / 2.0f, _Sprite.getLocalBounds().height / 2.0f);
	_Sprite.setPosition(pos);
	
}


Animation::~Animation()
{
}

void Animation::reset()
{
}

int Animation::getCurrentFrameNum()
{
	return _CurrentSprite.top / _CurrentSprite.height * _NumCols + _CurrentSprite.left / _CurrentSprite.width;
}

bool Animation::nextSprite()
{
	_CurrentSprite.left += _CurrentSprite.width;
	if (getCurrentFrameNum() == _NumFrames) {
		_CurrentSprite.left = 0;
		_CurrentSprite.top = 0;
		_Sprite.setTextureRect(_CurrentSprite);
		return false;
	}
	if (_CurrentSprite.left == _SpriteSheet.getSize().x) {
		_CurrentSprite.left = 0;
		_CurrentSprite.top += _CurrentSprite.height;
		
		if (_CurrentSprite.top == _SpriteSheet.getSize().y) {
			_CurrentSprite.left = 0;
			_CurrentSprite.top = 0;
			_Sprite.setTextureRect(_CurrentSprite);
			return false;
		}
	}
	_Sprite.setTextureRect(_CurrentSprite);
	return true;
}
