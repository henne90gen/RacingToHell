#include "stdafx.h"
#include "Animation/Animation.h"

Animation::Animation(sf::Vector2f pos, sf::Texture & spriteSheet, float timePerFrame, int frames, int rows, int cols) : 
	_AnimState(Animation::Play), _TimePerFrame(timePerFrame), _NumFrames(frames), _NumRows(rows), _NumCols(cols)
{
	_SpriteSheet = spriteSheet;
	_Sprite.setTexture(_SpriteSheet);
	_CurrentSpriteRect = sf::IntRect(0, 0, _Sprite.getLocalBounds().width / _NumCols, _Sprite.getLocalBounds().height / _NumRows);
	_Sprite.setTextureRect(_CurrentSpriteRect);
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
	return _CurrentSpriteRect.top / _CurrentSpriteRect.height * _NumCols + _CurrentSpriteRect.left / _CurrentSpriteRect.width;
}

bool Animation::nextFrame()
{
	_CurrentSpriteRect.left += _CurrentSpriteRect.width;
	// switches back to the first frame if the maximum number of frames is reached
	if (getCurrentFrameNum() == _NumFrames) {
		_CurrentSpriteRect.left = 0;
		_CurrentSpriteRect.top = 0;
		_Sprite.setTextureRect(_CurrentSpriteRect);
		return false;
	}
	if (_CurrentSpriteRect.left == _SpriteSheet.getSize().x) {
		_CurrentSpriteRect.left = 0;
		_CurrentSpriteRect.top += _CurrentSpriteRect.height;
	}
	_Sprite.setTextureRect(_CurrentSpriteRect);
	return true;
}
