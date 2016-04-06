#pragma once
#include "GameObject/Boss/BossCar.h"

class Tank : public BossCar
{
public:
	/*
		Tank boss
		@param texture Texture for the sprite
		@param bulletTexture Texture for all bullets the tank shoots
	*/
	Tank(unsigned int id, int difficulty, int HP, sf::Texture& Texture, sf::Texture& BulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBufferShot, sf::SoundBuffer &soundBufferExplosion, float volume);
	Tank(std::istream& stream, sf::Texture& Texture, sf::Texture& BulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBufferShot, sf::SoundBuffer &soundBufferExplosion, float volume);
	~Tank() {}

	/*
		See BossCar for details
	*/
	void render(sf::RenderWindow& RenderWindow);

	/*
		See BossCar for details
	*/
	void update(float FrameTime, int RoadSpeed, std::vector<std::shared_ptr<GameObject>>& gameObjects);
	
	void init();
private:
	sf::Texture _GunTexture;
	sf::Sprite _GunSprite;

	bool _MovementSwitch;
};

