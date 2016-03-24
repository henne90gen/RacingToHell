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
	Tank(sf::Texture& Texture, sf::Texture& BulletTexture);
	~Tank() {}

	/*
		See BossCar for details
	*/
	void render(sf::RenderWindow& RenderWindow);

	/*
		See BossCar for details
	*/
	void update(float FrameTime, int RoadSpeed, std::vector<std::shared_ptr<GameObject>>& gameObjects);
	
private:
	sf::Texture _GunTexture;
	sf::Sprite _GunSprite;
	
	float _Radius;

	bool _MovementSwitch;
};

