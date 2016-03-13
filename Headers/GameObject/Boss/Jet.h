#pragma once

#include "GameObject\Boss\BossCar.h"

class Jet : public BossCar
{
public:
	Jet(sf::Texture& Texture, sf::Texture* BulletTexture);
	~Jet();

	void update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects);
	void render(sf::RenderWindow& RenderWindow);
private:
	void RandomPosition();
};
