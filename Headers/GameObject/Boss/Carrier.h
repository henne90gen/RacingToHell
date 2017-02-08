#pragma once
#include "GameObject/Boss/BossCar.h"
#include "GameObject/GameObjectFactory.h"

class Carrier : public BossCar
{
public:
	/*
		Carrier boss
		@param texture Texture for the sprite
		@param bulletTexture Texture for all bullets the boss shoots
	*/
    Carrier(unsigned int id, int difficulty, int HP, sf::Texture &texture, sf::Texture &bulletTexture);

    Carrier(sf::Packet &packet, sf::Texture &texture, sf::Texture &bulletTexture);
	virtual ~Carrier() {}

	/*
		See BossCar for details
	*/
	void update(float FrameTime, int RoadSpeed, std::vector<std::shared_ptr<Bullet>>& bullets, PlayerCar& player);

	/*
		See BossCar for details
	*/
	void render(sf::RenderWindow& RenderWindow);

	void init();
private:
	sf::Texture _GunTexture;
	sf::Sprite _GunSprite;

	float _Radius;

	virtual void shootBullet(std::vector<std::shared_ptr<Bullet>>& bullets, sf::Vector2f pos, sf::Vector2f dir, int BulletSpeed, float volume);

	bool _MovementSwitchLeftRight, _MovementSwitchUpDown, _SwitchSides;
	sf::Clock _SwitchSidesClock;
	float _SwitchSideTime;

	void checkPhase();
};
