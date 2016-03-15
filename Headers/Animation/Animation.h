#pragma once
class Animation
{
public:
	/*
		Animation is the interface for all animations. It handles creation and sprite updating.
		@param pos			Position of the center of the animation
		@param spriteSheet	Texture to use as a sprite sheet
		@param timePerFrame Time each frame of the animation will be displayed
		@param frames		Total number of frames in the sprite sheet
		@param rows			Number of rows the sprite sheet has
		@param cols			Number of columns the sprite sheet has
	 */
	Animation(sf::Vector2f pos, sf::Texture & texture, float timePerFrame, int frames, int rows, int cols);
	~Animation();
	
	/*
		State an animation may be in
	 */
	enum AnimationState {
		Stop, Play, Pause
	};

	/*
		Renders the animation to the specified RenderWindow
		@param window Window to draw to
	*/
	virtual void render(sf::RenderWindow & window) = 0;

	/*
		Updates the animation with the given frame time
		@param frameTime Time that has passed since the last update
	 */
	virtual void update(float frameTime) = 0;

	/*
		Starts playing the animation
	*/
	void play() { _AnimState = Animation::Play; }

	/*
		Pauses the animation
	*/
	void pause() { _AnimState = Animation::Pause; }

	/*
		Stops the animation
	*/
	void stop() { _AnimState = Animation::Stop; }

	/*
		Resets the animation to the beginning
	*/
	void reset();

	/*
		Returns the state the animation is currently in
		@return AnimationState of the animation
	*/
	AnimationState getAnimationState() { return _AnimState; }

	/*
		Returns the number of the frame that is currently being displayed
		@return int Number of current frame
	*/
	int getCurrentFrameNum();

	/*
		Returns the sprite of the animation, which is the size of a single frame from the supplied sprite sheet
		@return sf::Sprite of the animation
	*/
	sf::Sprite & getSprite() { return _Sprite; }

	/*
		Sets the rotation of the animation sprite to the given angle
		@param angle Angle in degrees
	*/
	void setRotation(float angle) { _Sprite.setRotation(angle); }

	/*
		Returns the current angle of rotation of the animation sprite
		@return float Angle in degrees
	*/
	float getRotation() { return _Sprite.getRotation(); }

protected:

	/*
		Switches to the next frame in the sprite sheet and switches back to the first if the end is reached
		@return bool whether there is a next frame in the sprite sheet
	*/
	bool nextFrame();

	sf::Sprite _Sprite;
	sf::Texture _SpriteSheet;
	sf::IntRect _CurrentSpriteRect;

	int _NumFrames, _NumRows, _NumCols;

	sf::Clock _Timer;
	float _ElapsedTime, _TimePerFrame;
	AnimationState _AnimState;
};

