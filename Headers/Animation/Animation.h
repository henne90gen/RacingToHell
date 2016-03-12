#pragma once
class Animation
{
public:
	Animation(sf::Vector2f pos, sf::Texture & texture, float timePerFrame, int frames, int rows, int cols);
	~Animation();

	enum AnimationState {
		Stop, Play, Pause
	};

	virtual void render(sf::RenderWindow & Window) = 0;
	virtual void update(float FrameTime) = 0;

	void play() { _AnimState = Animation::Play; }
	void pause() { _AnimState = Animation::Pause; }
	void stop() { _AnimState = Animation::Stop; }
	void reset();

	AnimationState getAnimationState() { return _AnimState; }
	sf::Sprite & getSprite() { return _Sprite; }
protected:
	bool nextSprite();

	sf::Sprite _Sprite;
	sf::Texture _SpriteSheet;
	sf::IntRect _CurrentSprite;

	int _NumFrames, _NumRows, _NumCols;

	sf::Clock _Timer;
	float _ElapsedTime, _TimePerFrame;
	AnimationState _AnimState;
};

