#pragma once
#include "Animation/Animation.h"

class Explosion : public Animation
{
public:
	/*
		Explosion animation
		@param pos Position of the center of the explosion
		@param texture Sprite sheet
		@param movement Vector that is going to be applied to the explosion to make it move
		@param soundVector Vector of all the sounds
	*/
	Explosion(sf::Vector2f pos, sf::Texture & texture, sf::Vector2f movement, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBuffer, float Volume);
	~Explosion() {}

	/*
		See Animation for details
	*/
	virtual void render(sf::RenderWindow & Window);

	/*
		See Animation for details
	*/
	virtual void update(float FrameTime);

private:
	sf::Vector2f _Movement;

	/*
	Plays a explosion sound
	@param position Position of the explosion
	*/
	void playExplosionSound(sf::Vector2f position, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBuffer, float Volume);
};

