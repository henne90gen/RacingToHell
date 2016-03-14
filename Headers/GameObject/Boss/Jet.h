#pragma once
#include "GameObject\Boss\BossCar.h"

class Jet : public BossCar
{
public:
	/*
		Jet boss
		@param texture Texture for the sprite
		@param bulletTexture Texture for all bullets the Jet shoots
	*/
	Jet(sf::Texture& texture, sf::Texture& bulletTexture);
	~Jet();

	/*
		See BossCar for details
	*/
	void update(float frameTime, int roadSpeed, std::vector<GameObject*>& gameObjects);

	/*
		See BossCar for details
	*/
	void render(sf::RenderWindow& window);

private:

	/*
		Sets the Jet to a random position off screen and sets the next position on the opposite site
	*/
	void randomPosition();

	/*
		Changes to the next phase
	*/
	void checkPhase();
};
