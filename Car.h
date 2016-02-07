#pragma once

#include "GameObject.h"

class Car : public GameObject
{
public:
	Car();
	~Car();


	//Getters and Setters
	sf::Vector2f getPos() { return  _Sprite.getPosition(); }
	void setPos(sf::Vector2f Pos) { _Sprite.setPosition(Pos); }
	float getWidth() { return _Sprite.getLocalBounds().width; }
	float getHeight() { return _Sprite.getLocalBounds().height; }
	int  getHealth() { return _Health; }
	void setHealth(int Health) { _Health = Health; }

	//Inherited functions from GameObject
	void render(sf::RenderWindow& Window);
	void handleEvent(sf::Event& Event);
	void update(float FrameTime);
	
	//Variables

private:
	sf::Sprite _Sprite;
	sf::Texture _Texture;
	int _Health;
	float _Speed;

	sf::Vector2f _Movement;

};

