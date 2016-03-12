#pragma once
#include "GameObject/Boss/BossCar.h"
#include "Animation/MechTop.h"
#include "Animation/MechLegs.h"

class Mech : public BossCar
{
public:
	Mech(sf::Texture & TextureTop, sf::Texture & TextureLegs, sf::Texture* BulletTexture);
	~Mech();

	void render(sf::RenderWindow & Window);
	void update(float FrameTime, int RoadSpeed, std::vector<GameObject*> & GameObjects);

private:
	MechTop _TopAnim;
	MechLegs _LegsAnim;
};

