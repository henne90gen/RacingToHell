#pragma once
#include "GameObject/Boss/BossCar.h"

class Carrier : public BossCar
{
public:
	Carrier(sf::Texture& Texture, sf::Texture* BulletTexture);
	~Carrier();

	void update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects);
	void render(sf::RenderWindow& RenderWindow);
private:
	sf::Texture _GunTexture;
	sf::Sprite _GunSprite;

	sf::Vector2f _GunPosition;
	float _GunOrientation, _Radius;

	bool _MovementSwitchLeftRight, _MovementSwitchUpDown;
	sf::Clock _SwitchSidesClock;
	float _SwitchSideTime;

	sf::Vector2f _DefaultPosition;

	sf::Vector2f& calcBulletPosition();
};