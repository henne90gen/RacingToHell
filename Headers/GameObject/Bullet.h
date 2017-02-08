#pragma once

#include "GameObject/GameObject.h"
#include "GameObject/GameObjectType.h"

class Bullet : public GameObject
{
public:
    /*
        Bullet that can be shot by any Car
        @param pos Starting position of the Bullet
        @param dir Angle at which the Bullet will fly
        @param speed Speed of the Bullet
        @param type Specifies the type of the Bullet, thus who shot it
        @param texture Texture that is going to be used for the sprite
        @param soundEffects Vector to register the sound of the shot in
        @param volume VolumeIndex of the shot sound
    */
    Bullet(unsigned int id, sf::Vector2f pos, sf::Vector2f dir, float speed, GameObjectType type,
           sf::Texture &texture);

    Bullet(sf::Packet &packet, GameObjectType type, sf::Texture &texture);
	Bullet(sf::Packet &packet, sf::Vector2f PlayerPosition, GameObjectType type, sf::Texture& texture, sf::Uint32 ID, float Speed);
	Bullet(GameObjectType type, sf::Texture &texture, float speed);
	~Bullet() {}

	/*
		Handles events for Bullet
		@param newEvent Event to be handled
	*/
	void handleEvent(sf::Event &newEvent) {}

	/*
		Updates the Bullet with the given frame time
		@param frameTime Time that has passed since the last update
		@param roadSpeed Speed of the road
	*/
	void update(float frameTime, int roadSpeed);

	/*
		Writes the necessary data for a gameobject to a packet
	*/
	void operator>>(sf::Packet& packet);

	/*
		Reads the necessary data for a gameobject from a packet
	*/
	void operator<<(sf::Packet& packet);

	sf::Vector2f getDir() { return _Direction; }
private:
	float _Speed;
	sf::Vector2f _Direction;

	void playShotSound(sf::Vector2f position, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBuffer, float Volume);
};
