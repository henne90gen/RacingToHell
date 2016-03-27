#pragma once

#include "GameObject\Car.h"
#include "GameObject\GameObjectType.h"
#include "GameObject\Bullet.h"
#include "Animation\Explosion.h"

class BossCar : public Car
{
public:
	/*
		BossCar is the interface for every boss. It handles creation and sprite updating.
		@param pos				Starting position
		@param difficulty		selected difficulty
		@param health			Starting health of the boss 
		@param texture			Texture for the sprite that is going to be used for collision detection
		@param bulletTexture	Texture for the bullets the boss will shoot
	*/
	BossCar(sf::Vector2f& pos, int difficulty, int health, float speed, sf::Texture& texture, sf::Texture& bulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBufferShot, sf::SoundBuffer &soundBufferExplosion, float Volume);
	~BossCar() {}

	/*
		Renders the boss to the specified RenderWindow
		@param window Window to draw to
	*/
	virtual void render(sf::RenderWindow& window) = 0;

	/*
		Updates the boss with the given frame time
		@param frameTime Time that has passed since the last update
		@param roadSpeed Velocity of the road
		@param gameObjects Reference to the vector with all the game objects, so that the boss can add the bullets it shoots
	*/
	virtual void update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<GameObject>>& gameObjects) = 0;

	/*
		Returns true if the boss allows traffic
	*/
	bool getTraffic() { return _Traffic; }

	/*
		Returns true if the boss is dead and all explosion animations are done playing
	*/
	bool isDoneExploding(sf::Texture& ExplosionTexture);
protected:
	enum Phase { NOTHING, SIMPLESHOOT, SALVE, SPIN, HARDCORESPAM, BLASTSALVE, SPIRAL, RANDOMSPRAY, SHOTGUN, SIDE, SAVELANES, RUNATPLAYERPHASE, ZICKZACKPHASE, SALVEZICKZACK };
	int _CurrentPhase;
	std::vector<std::pair<Phase, float>> _Pattern;
	sf::Clock _PhaseClock;

	std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>> &_soundEffects;
	sf::SoundBuffer &_soundBufferShot, &_soundBufferExplosion;
	float _Volume;

	int _Difficulty;

	enum Movement { STILL, DRIVETODEFAULT, LEFTRIGHT, SWITCHSIDES, STRAIGHT, PARABOLA, RUNATPLAYER, ZICKZACK};
	Movement _Movement;

	sf::Vector2f _GunPosition;
	sf::Vector2f _GunOrientation;
	float _GunLength;

	sf::Vector2f _DefaultPosition;

	sf::RectangleShape _HealthBar;
	sf::RectangleShape _HealthBarFrame;

	sf::Texture _BulletTexture;

	int const _BulletSpeed;

	std::vector<Explosion*> _Explosions;
	sf::Clock _ExplosionTimer;
	int _IsExploding;

	sf::Clock _BossEventTimer1;
	sf::Clock _BossEventTimer2;
	float _Event1Frequency, _Event2Frequency;
	bool _Event1Switch, _Event2Switch;
	int _Event1Counter, _Event2Counter;

	sf::Vector2f _NextPosition;
	bool _Attack, _Traffic;

	float PlayerAngle(GameObject& Player);
	void shootBullet(std::vector<std::shared_ptr<GameObject>>& GameObjects, sf::Vector2f pos, sf::Vector2f dir) { shootBullet(GameObjects, pos, dir, _BulletSpeed); }
	void shootBullet(std::vector<std::shared_ptr<GameObject>>& GameObjects, sf::Vector2f pos, sf::Vector2f dir, int BulletSpeed);
	bool driveToNextPosition(float FrameTime);
	int getBossEvent();
	void updateHealthBar();
	virtual void checkPhase();
	void renderExplosions(sf::RenderWindow& Window);
	void updateExplosions(float FrameTime);

	/*
		Calculates the position where a bullet can be spawned
		@return sf::Vector2f Position for the bullet
	*/
	virtual sf::Vector2f calcBulletPosition();
};