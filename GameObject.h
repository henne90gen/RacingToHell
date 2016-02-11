#pragma once

#include "GameObjects.h"

#include <string>

class GameObject
{
public:
	GameObject(GameObjects Type, std::string Filename);
	~GameObject();

	virtual void render(sf::RenderWindow& Window);
	virtual void handleEvent(sf::Event& Event) = 0;
	virtual void update(float FrameTime) = 0;

	GameObjects getType() { return _Type; }
	sf::Vector2f getPos() { return  _Sprite.getPosition(); }
	float getWidth() { return _Sprite.getLocalBounds().width; }
	float getHeight() { return _Sprite.getLocalBounds().height; }
	sf::Sprite getSprite() { return _Sprite; }

	void setPos(sf::Vector2f Pos) { _Sprite.setPosition(Pos); }
private:
	sf::Sprite _Sprite;
	sf::Texture _Texture;
	
	GameObjects _Type;
};

