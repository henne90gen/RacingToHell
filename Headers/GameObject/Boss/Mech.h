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

	void setPos(sf::Vector2f pos);
	sf::Vector2f getPos() { return _TopAnim.getSprite().getPosition(); }

	float getWidth() { return _TopAnim.getSprite().getLocalBounds().width; }
	float getHeight() { return _TopAnim.getSprite().getLocalBounds().height; }

	sf::Sprite& getSprite() { return _TopAnim.getSprite(); }
private:
	MechTop _TopAnim;
	MechLegs _LegsAnim;

	bool _MovementSwitch;
};

