#pragma once
#include "GameObject\BossCar.h"

class Tank : public BossCar
{
public:
	Tank(sf::Texture& Texture);
	~Tank();

	void update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects);
	void render(sf::RenderWindow& RenderWindow);
private:
	sf::Texture _GunTexture;
	sf::Sprite _GunSprite;
	
	sf::Vector2f _GunPosition;
	float _Orientation, _Radius;

};

