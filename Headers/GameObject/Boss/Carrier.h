#pragma once
#include "GameObject/Boss/BossCar.h"

class Carrier : public BossCar
{
public:
	/*
		Carrier boss
		@param texture Texture for the sprite
		@param bulletTexture Texture for all bullets the boss shoots
	*/
	Carrier(int difficulty, int HP, sf::Texture& texture, sf::Texture& bulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBufferShot, sf::SoundBuffer &soundBufferExplosion, float Volume);
	~Carrier() {}

	/*
		See BossCar for details
	*/
	void update(float FrameTime, int RoadSpeed, std::vector<std::shared_ptr<GameObject>>& GameObjects);

	/*
		See BossCar for details
	*/
	void render(sf::RenderWindow& RenderWindow);
private:
	sf::Texture _GunTexture;
	sf::Sprite _GunSprite;

	float _Radius;

	bool _MovementSwitchLeftRight, _MovementSwitchUpDown;
	sf::Clock _SwitchSidesClock;
	float _SwitchSideTime;
};