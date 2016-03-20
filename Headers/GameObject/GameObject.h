#pragma once

#include "GameObject/GameObjectType.h"
#include "Collision.h"

class GameObject
{
public:
	/*
		Any object that can be on the screen
		@param pos Position of the GameObject
		@param type Type of the GameObject
		@param texture Texture that is going to be used for the Sprite
	*/
	GameObject(sf::Vector2f pos, GameObjectType type, sf::Texture& texture);
	~GameObject() {}

	/*
		Renders the GameObject to the specified RenderWindow
		@param window Window to draw to
	*/
	virtual void render(sf::RenderWindow& window);

	/*
		Renders the GameObject to the specified RenderWindow (PlayerCar)
		@param window Window to draw to
		@param renderCrosshair States whether or not the crosshair should be drawn
	*/
	virtual void render(sf::RenderWindow& window, bool renderCrosshair) {};

	/*
		Handles events for GameObject
		@param newEvent Event to be handled
	*/
	virtual void handleEvent(sf::Event& newEvent) {}

	/*
		Updates the GameObject with the given frame time
		@param frameTime Time that has passed since the last update
		@param roadSpeed Velocity of the road
	*/
	virtual void update(float frameTime, int roadSpeed);

	/*
		Updates the GameObject with the given frame time (Boss)
		@param frameTime Time that has passed since the last update
		@param roadSpeed Velocity of the road
		@param GameObjects GameObject-Vector
	*/
	virtual void update(float FrameTime, int roadSpeed, std::vector<std::shared_ptr<GameObject>>& GameObjects) {};

	/*
		@return GameObjectTyoe The type of the GameObject
	*/
	GameObjectType getType() { return _Type; }

	/*
		@return Vectorf2f The position of the GameObject on the screen
	*/
	virtual sf::Vector2f getPos() { return  _Sprite.getPosition(); }

	/*
		@return float The Width of the sprite of the GameObject
	*/
	virtual float getWidth() { return _Sprite.getLocalBounds().width; }

	/*
		@return float The Height of the sprite of the GameObject
	*/
	virtual float getHeight() { return _Sprite.getLocalBounds().height; }

	/*
		@return float The Sprite of the GameObject
	*/
	virtual sf::Sprite& getSprite() { return _Sprite; }

	/*
		Sets the sprite of the GameObject
		@param sprite New sprite for this GameObject
	*/
	void setSprite(sf::Sprite & sprite) { _Sprite = sprite; }

	/*
		Sets the position of the GameObject
		@param pos New position for this GameObject
	*/
	virtual void setPos(sf::Vector2f pos) { _Sprite.setPosition(pos); }

	/*
		Sets the texture of the GameObject
		@param texture New texture for this GameObject
	*/
	void setTexture(sf::Texture& texture);

	/*
		Changes the color of the sprite
		@param color New color for the sprite
	*/
	void setSpriteColor(sf::Color color) { _Sprite.setColor(color); }

	/*
		Compares this GameObject with another one to see if they are colliding
		@param go GameObject to compare this one to
		@return bool True if the two GameObjects collide
	*/
	bool checkForCollision(GameObject& go);

	/*
		Current lane the car is on
		@return int Number of the lane
	*/
	virtual int getLane() { return -1; }

	/*
		@return int Speed of the Car
	*/
	virtual int getSpeed() { return -1; }

	/*
		Sets the speed of the car
		@param int New speed of the car
	*/
	virtual void setSpeed(int Speed) {};

	/*
		@return int Health of the Car
	*/
	virtual int getHealth() { return -1; }

	/*
		@return int Maximum-Health of the Car
	*/
	virtual int getMaxHealth() { return -1; }

	/*
		Damage the car and subtract the damage from the health
		@param damage Amount of health to subtract
	*/
	virtual void takeDamage(int Damage) {}

	/*
		Adds 50 to the players energy if possible
	*/
	virtual void addEnergy() {}

	/*
		@return float Energy of the player
	*/
	virtual float getEnergy() { return -1; }

	/*
		@return float Maximum amount of energy the player can have
	*/
	virtual int getMaxEnergy() { return -1; }

	/*
		Adds 20 to the players health if possible
	*/
	virtual void addHealth() {}

	/*
		@return int Damage of a bullet fired by the player
	*/
	virtual int getBulletdamage() { return -1; }

	/*
		Resets angle for new bullets so that no new bullets are fired
	*/
	virtual void resetShotBullet() {};

	/*
		Gives back the angle of the shot bullet or 360.0f if no bullet was fired
		@return float Angle of the shot bullet
	*/
	virtual float shotBullet() { return -1.0f; }

	/*
		Changes the players stats according to the id of the car he chose.
	*/
	virtual void setStats(int CurrentIndex) {}

	/*
		Returns true if the boss allows traffic
	*/
	virtual bool getTraffic() { return false; }

	/*
		Returns true if the boss is dead and all explosion animations are done playing
	*/
	virtual bool isDoneExploding(sf::Texture& ExplosionTexture) { return false; }
private:
	sf::Sprite _Sprite;
	sf::Texture _Texture;
	
	GameObjectType _Type;
};

