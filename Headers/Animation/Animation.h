#pragma once
class Animation
{
public:
	Animation(sf::Vector2f pos, std::string path, float timePerFrame);
	~Animation();

	enum AnimationState {
		Stop, Play, Pause
	};

	virtual void render(sf::RenderWindow & Window) = 0;
	virtual void update(float FrameTime) = 0;

	void play() { _AnimState = Animation::Play; }
	void pause() { _AnimState = Animation::Pause; }
	void stop() { _AnimState = Animation::Stop; }

	AnimationState getAnimationState() { return _AnimState; }
protected:
	bool nextSprite();

	sf::Sprite _Sprite;
	sf::Texture _SpriteSheet;
	sf::IntRect _CurrentSprite;

	sf::Clock _Timer;
	float _ElapsedTime, _TimePerFrame;
	AnimationState _AnimState;
};

