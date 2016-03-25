#pragma once
#include "GameObject/Boss/BossCar.h"
#include "Animation/MechTop.h"
#include "Animation/MechLegs.h"

class Mech : public BossCar
{
public:
	/*
		Mech boss
		@param texture Texture for the sprite
		@param bulletTexture Texture for all bullets the Mech shoots
	*/
	Mech(sf::Texture& TextureTop, sf::Texture& TextureLegs, sf::Texture& BulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBufferShot, sf::SoundBuffer &soundBufferExplosion, float Volume);
	~Mech() {}

	/*
		See BossCar for details
	*/
	void render(sf::RenderWindow & Window);

	/*
		See BossCar for details
	*/
	void update(float FrameTime, int RoadSpeed, std::vector<std::shared_ptr<GameObject>> & GameObjects);

	/*
		Overriding setPos() of GameObject
		Changes the position of the Mech
		@param pos New position of the Mech
	*/
	void setPos(sf::Vector2f pos);

	/*
		Overriding getPos() of GameObject
		@return sf::Vector2f Position of the center of the Mech
	*/
	sf::Vector2f getPos() { return _TopAnim.getSprite().getPosition(); }

	/*
		Overriding setWidth() of GameObject
		@return float Width of the Mech
	*/
	float getWidth() { return _TopAnim.getSprite().getLocalBounds().width; }
	
	/*
		Overriding setHeight() of GameObject
		@return float Height of the Mech
	*/
	float getHeight() { return _TopAnim.getSprite().getLocalBounds().height; }

	/*
		Overriding setSprite() of GameObject
		@return sf::Sprite Sprite of the Mech
	*/
	sf::Sprite& getSprite() { return _TopAnim.getSprite(); }
private:
	MechTop _TopAnim;
	MechLegs _LegsAnim;

	bool _MovementSwitch;
	
	float _GunRadius;

	/*
		Calculates the positions of the two guns of the Mech
		@return pair<Vector2f, Vector2f> First is the position of the left gun and second is the position of the right gun
	*/
	std::pair<sf::Vector2f, sf::Vector2f>& calcGunPositions();
	sf::Vector2f calcBulletPosition() { return sf::Vector2f(); }
};

