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
	Carrier(sf::Texture& texture, sf::Texture& bulletTexture);
	~Carrier();

	/*
		See BossCar for details
	*/
	void update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects);

	/*
		See BossCar for details
	*/
	void render(sf::RenderWindow& RenderWindow);
private:
	sf::Texture _GunTexture;
	sf::Sprite _GunSprite;

	sf::Vector2f _GunPosition;
	float _GunOrientation, _Radius;

	bool _MovementSwitchLeftRight, _MovementSwitchUpDown;
	sf::Clock _SwitchSidesClock;
	float _SwitchSideTime;

	/*
		Calculates the position where a bullet can be spawned
		@return sf::Vector2f Position for the bullet
	*/
	sf::Vector2f& calcBulletPosition();
};