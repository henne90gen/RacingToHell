#pragma once

#include "GameObjects.h"

#include <string>

class GameObject
{
public:
	GameObject(sf::Vector2f pos, GameObjects Type, sf::Texture& texture);
	~GameObject();

	virtual void render(sf::RenderWindow& Window);
	virtual void handleEvent(sf::Event& Event) = 0;
	virtual void update(float FrameTime, int RoadSpeed) = 0;

	GameObjects getType() { return _Type; }
	sf::Vector2f getPos() { return  _Sprite.getPosition(); }
	float getWidth() { return _Sprite.getLocalBounds().width; }
	float getHeight() { return _Sprite.getLocalBounds().height; }
	sf::Sprite& getSprite() { return _Sprite; }

	void setPos(sf::Vector2f Pos) { _Sprite.setPosition(Pos); }
	void setSkin(sf::Texture* skin);
	void setSpriteColor(sf::Color Color) { _Sprite.setColor(Color); }

	bool checkForCollision(GameObject* go);
private:
	sf::Sprite _Sprite;
	sf::Texture _Texture;
	
	GameObjects _Type;
};

