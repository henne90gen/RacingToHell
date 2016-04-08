#pragma once
#include "GameObject\Boss\BossCar.h"
#include "GameObject\GameObjectFactory.h"

class Jet : public BossCar
{
public:
	/*
		Jet boss
		@param texture Texture for the sprite
		@param bulletTexture Texture for all bullets the Jet shoots
	*/
	Jet(unsigned int id, int difficulty, int HP, sf::Texture& texture, sf::Texture& bulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBufferShot, sf::SoundBuffer &soundBufferExplosion, sf::SoundBuffer &soundBufferEngine, float Volume);
	Jet(std::istream& stream, sf::Texture& texture, sf::Texture& bulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBufferShot, sf::SoundBuffer &soundBufferExplosion, sf::SoundBuffer &soundBufferEngine, float Volume);
	Jet(sf::Packet& packet, sf::Texture& texture, sf::Texture& bulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBufferShot, sf::SoundBuffer &soundBufferExplosion, sf::SoundBuffer &soundBufferEngine, float Volume);
	~Jet() {}

	/*
		See BossCar for details
	*/
	void update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<GameObject>>& GameObjects);

	/*
		See BossCar for details
	*/
	void render(sf::RenderWindow& window);

	void stopSounds();

	void init();
private:

	/*
		Sets the Jet to a random position off screen and sets the next position on the opposite site
	*/
	void randomPosition();

	/*
		Changes to the next phase
	*/
	void checkPhase();

	virtual void shootBullet(std::vector<std::shared_ptr<GameObject>>& GameObjects, sf::Vector2f pos, sf::Vector2f dir, int BulletSpeed, float volume);

	sf::SoundBuffer _EngineSoundBuffer;
	sf::Sound _EngineSound;
};
