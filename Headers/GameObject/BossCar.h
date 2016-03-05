#pragma once
#include "GameObject\Car.h"

class BossCar : public Car
{
public:
	BossCar(std::pair<sf::Texture*, sf::Texture*> textures);
	~BossCar();

	void render(sf::RenderWindow& Window);
	void update(float FrameTime, int RoadSpeed);
private:
	sf::Texture _CannonTexture;
	sf::Sprite _CannonSprite;
};

